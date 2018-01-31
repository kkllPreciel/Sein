/**
 *  @file     direct3d12_device.cc
 *  @brief    Direct3D12デバイスに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/02/07
 *  @version  1.0
 */

#if _DEBUG
#include <sstream>
#endif
#include <winerror.h>
#include <d3dcompiler.h>
#include "direct3d12_device.h"
#include "depth_stencil_view.h"
#include "fence.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "constant_buffer.h"
#include "shader_resource_buffer.h"
#include "descriptor_heap.h"
#include "descriptor.h"
#include "command_list.h"
#include "buffer.h"
#include "texture.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コンストラクタ
     */
    Device::Device() :
      device(nullptr, [](IUnknown* p) { p->Release(); }), swapChain(nullptr, [](IUnknown* p) { p->Release(); }), commandQueue(nullptr, [](IUnknown* p) { p->Release(); }), commandList(nullptr),
      descriptorHeaps(nullptr), bufferIndex(0), rootSignature(nullptr), pipelineState(nullptr),
      depthStencilView(nullptr), fence(nullptr), texBuffer(nullptr)
    {
      for (auto i = 0; i < FrameCount; ++i)
      {
        renderTargetList[i] = nullptr;
      }
    }

    /**
     *  @brief  デストラクタ
     */
    Device::~Device()
    {
      Release();
    }

    /**
     *  @brief  デバイスを生成する
     *  @param  handle:ウィンドウハンドル
     *  @param  width:横幅
     *  @param  height:縦幅
     */
    void Device::Create(HWND handle, unsigned int width, unsigned int height)
    {
#if _DEBUG
      // デバッグレイヤーを有効に設定する
      Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
      if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
      {
        throw "デバッグレイヤーの生成に失敗しました。";
      }
      debugInterface->EnableDebugLayer();
#endif
      // ファクトリの生成
      // アダプターの列挙に使用
      Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

      if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
      {
        throw "DXGIファクトリの生成に失敗しました。";
      }

      // Direct3D12のデバイスを作成する
      {
#if 1
        // デバイスを生成
        // アダプターを列挙し作成すると
        // 環境によってメモリリークが発生したため
        // デフォルトのアダプターを使用し作成する
        ID3D12Device* pDevice;
        if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice))))
        {
          throw "デバイスの生成に失敗しました。";
        }
        device.reset(pDevice);
#else
        Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;

        // アダプター(ビデオカード)を列挙する
        for (auto i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pAdapter); ++i)
        {
          DXGI_ADAPTER_DESC1 desc;
          pAdapter->GetDesc1(&desc);

          // TODO:調査
          if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
          {
            continue;
          }

          // デバイスを作成
          if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
          {
            continue;
          }

#if _DEBUG
          std::wostringstream ostr;
          pAdapter->GetDesc1(&desc);
          ostr << "D3D12-capable hardware found:" << desc.Description << "(" << (desc.DedicatedVideoMemory >> 20) << "MB)\n";
          OutputDebugString(ostr.str().c_str());
#endif
        }

        // WARPで生成する
        if (nullptr == device)
        {
#if _DEBUG
          OutputDebugString(L"WARP software adapter requested.  Initializing...\n");
#endif
          if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter))))
          {
            throw "WARPアダプターが存在しません。";
          }

          if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
          {
            throw "デバイスの生成に失敗しました。";
          }
        }
#endif
      }

      // コマンドキューの作成
      // コマンドキューはGPUへ描画命令が出されたコマンドリストを
      // 順次実行する
      // つまり特定のアダプター(GPU)に関連付けられている
      {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;  // タイムアウト処理を有効にする
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;  // レンダリング関連のコマンドリスト

        ID3D12CommandQueue* queue;
        if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue))))
        {
          throw "コマンドキューの生成に失敗しました。";
        }
        commandQueue.reset(queue);
      }

      // スワップチェインの作成
      // コマンドキューを指定して作成する = アダプターを指定して作成する
      {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = width;                                  // ウィンドウ横幅
        swapChainDesc.Height = height;                                // ウィンドウ縦幅
        swapChainDesc.BufferCount = FrameCount;                       // バッファの数は2個(フロントバッファも含むらしいが、公式サンプル等を見るとバックバッファの数な気がする)
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // 恐らくバッファのフォーマット(4 成分、32 ビット符号なし整数)
        swapChainDesc.Scaling = DXGI_SCALING_NONE;                    // 画面サイズとバッファサイズが等しくない時の拡縮動作(拡大縮小は行わない)
        swapChainDesc.SampleDesc.Quality = 0;                         // マルチサンプリングの品質レベル
        swapChainDesc.SampleDesc.Count = 1;                           // ピクセル単位のマルチサンプリング数
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // バックバッファの使用目的及びCPUアクセスオプション(レンダーターゲットとして使用)
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // スワップチェインの動作オプション(モード切替可能に設定)
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // フロントバッファとバックバッファのスワップ挙動指定(バックバッファがディスプレイに表示されたら破棄する)

        Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain;
        if (FAILED(factory->CreateSwapChainForHwnd(
          commandQueue.get(), // コマンドキュー
          handle,             // ウィンドウハンドル
          &swapChainDesc,     // スワップチェインの設定情報
          nullptr,            // フルスクリーンスワップチェインの設定(ウィンドウモードで作成するのでnullptr)
          nullptr,            // TODO:調査
          &pSwapChain)))
        {
          throw "スワップチェインの生成に失敗しました。";
        }

        IDXGISwapChain3* chain;
        if (FAILED(pSwapChain.Get()->QueryInterface(IID_PPV_ARGS(&chain))))
        {
          throw "IDXGISwapChain3の生成に失敗しました。";
        }
        swapChain.reset(chain);

        // バックバッファの番号を取得する
        bufferIndex = swapChain->GetCurrentBackBufferIndex();
      }

      // コマンドリストの生成
      commandList = std::make_unique<CommandList>();
      commandList->Create(device.get(), D3D12_COMMAND_LIST_TYPE_DIRECT);

      // Alt + Enterでフルスクリーン化の機能を無効に設定
      factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);

      // ディスクリプターヒープ用メモリ領域の確保
      {
        descriptorHeaps = std::make_unique<DescriptorHeap[]>(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES);
      }

      // 定数バッファビュー、シェーダーリソースビュー用ディスクリプターヒープを生成
      {
        auto& cbvSrvHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
        D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
        cbvHeapDesc.NumDescriptors = 5;                                 // ディスクリプターヒープ内のディスクリプター数(定数バッファ、シェーダーリソース)
        cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      // 定数バッファ or シェーダーリソース(テクスチャ) or ランダムアクセス のどれかのヒープ
        cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // シェーダーからアクセス可
        cbvSrvHeap.Create(device.get(), cbvHeapDesc);
      }

      // 深度ステンシルビュー用ディスクリプターヒープを生成
      {
        auto& dsvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = 1;                      // ディスクリプターヒープ内のディスクリプター数(深度ステンシルビュー)
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;   // 深度ステンシルビューのヒープ
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // シェーダーからアクセス不可
        dsvDescriptorHeap.Create(device.get(), heapDesc);
      }

      // レンダーターゲット
      {
        auto& rtvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_RTV];

        // レンダーターゲットビュー用ディスクリプターヒープの作成
        // ディスクリプターはバッファの情報データ(テクスチャバッファ、頂点バッファ等)
        {
          D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
          rtvHeapDesc.NumDescriptors = FrameCount;              // ディスクリプターヒープ内のディスクリプター数(フロントバッファ、バックバッファ)
          rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;    // 種別はレンダーターゲットビュー
          rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // シェーダーから参照しない
          rtvDescriptorHeap.Create(device.get(), rtvHeapDesc);
        }

        // ディスクリプターの登録
        {
          // フレームバッファ数文登録する
          for (auto i = 0; i < FrameCount; ++i)
          {
            if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargetList[i]))))
            {
              throw "バックバッファの取得に失敗しました。";
            }

            const auto& descriptor = rtvDescriptorHeap.CreateDescriptor();

            // レンダーターゲットビュー用のディスクリプターを作成する
            // ディスクリプターヒープの領域に作成される
            device->CreateRenderTargetView(
              renderTargetList[i],  // レンダー ターゲットを表すID3D12Resourceへのポインタ
              nullptr,              // D3D12_RENDER_TARGET_VIEW_DESCへのポインタ
              descriptor.GetHandleForCPU()
            );
          }
        }
      }

      // フェンスの生成
      // Directx12ではGPUの描画の終了待ちを自動で行わない(同期が取れず画面がおかしくなる)
      // そのため同期を取るためのオブジェクト(フェンス)を作成する
      {
        fence = std::make_unique<Fence>();
        fence->Create(device.get());

        // 描画処理を行っている可能性があるので描画終了待ちを行う
        WaitForGpu();
      }

      // アセットを読み込む
      {
        LoadAssets(width, height);
      }
    }

    /**
     *	@brief	デバイスを開放する
     */
    void Device::Release()
    {
      // GPUの描画終了待ちを行う
      WaitForGpu();

      pipelineState->Release();
      rootSignature->Release();

      for (auto i = 0; i < FrameCount; ++i)
      {
        renderTargetList[i]->Release();
      }
    }

    /**
     *	@brief	シーンを開始する
     */
    void Device::BeginScene()
    {
      commandList->Begin();

      // バックバッファが描画ターゲットとして使用できるようになるまで待つ
      D3D12_RESOURCE_BARRIER barrier;
      barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// バリアはリソースの状態遷移に対して設置
      barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
      barrier.Transition.pResource = renderTargetList[bufferIndex];			// リソースは描画ターゲット
      barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;			// 遷移前はPresent
      barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		// 遷移後は描画ターゲット
      barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
      commandList->Get().ResourceBarrier(1, &barrier);

      // バックバッファを描画ターゲットとして設定する
      // デバイスへ深度ステンシルビューをバインドする
      auto& rtvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_RTV];
      const auto& rtvDescriptor = rtvDescriptorHeap.GetDescriptor(bufferIndex);
      auto& dsvDescriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
      const auto& dsvDescriptor = dsvDescriptorHeap.GetDescriptor(0); // TODO:マジックナンバーを消す
      commandList->Get().OMSetRenderTargets(1, &rtvDescriptor.GetHandleForCPU(), false, &dsvDescriptor.GetHandleForCPU());

      // バックバッファをクリアする
      const float Color[] = { 0.0f, 0.0f, 0.6f, 1.0f };
      commandList->Get().ClearRenderTargetView(rtvDescriptor.GetHandleForCPU(), Color, 0, nullptr);

      // 深度ステンシルビューをクリアする(深度バッファのみ)
      commandList->Get().ClearDepthStencilView(dsvDescriptor.GetHandleForCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    }

    /**
     *	@brief	シーンを終了する
     */
    void Device::EndScene()
    {
      // バックバッファの描画完了を待つためのバリアを設置
      D3D12_RESOURCE_BARRIER barrier;
      barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;					// バリアはリソースの状態遷移に対して設置
      barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
      barrier.Transition.pResource = renderTargetList[bufferIndex];			// リソースは描画ターゲット
      barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// 遷移前は描画ターゲット
      barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// 遷移後はPresent
      barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
      commandList->Get().ResourceBarrier(1, &barrier);

      // コマンドリストをクローズする
      commandList->End();
    }

    /**
     *	@brief	画面を更新する
     */
    void Device::Present()
    {
      // コマンドリストの実行
      ID3D12CommandList* ppCommandLists[] = { &(commandList->Get()) };
      commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

      // 描画終了待ちを行う
      WaitForGpu();

      // 画面の更新
      if (FAILED(swapChain->Present(1, 0)))
      {
        throw "画面の更新に失敗しました。";
      }

      // バッファ番号を更新
      bufferIndex = swapChain->GetCurrentBackBufferIndex();
    }

    /**
     *	@brief	描画終了待ちを行う
     */
    void Device::WaitForGpu()
    {
      fence->Wait(commandQueue.get());
    }

    /**
     *	@brief	アセットを読み込む
     *	@param	width:ウィンドウ横幅
     *	@param	height:ウィンドウ縦幅
     */
    void Device::LoadAssets(unsigned int width, unsigned int height)
    {
      // 深度ステンシルビューの作成
      {
        depthStencilView = std::make_unique<DepthStencilView>();
        auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
        auto& descriptor = descriptorHeap.CreateDescriptor();
        depthStencilView->Create(device.get(), descriptor.GetHandleForCPU(), width, height);
      }

      // ルートシグネチャの作成
      {
        // ディスクリプターレンジの設定(定数バッファとシェーダーリソース(頂点シェーダーのみで使用可)とシェーダーリソース(ピクセルシェーダーのみで使用可))
        D3D12_DESCRIPTOR_RANGE descriptorRanges[3];
        {
          // 定数バッファ用ディスクリプターレンジの設定
          descriptorRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;								// ディスクリプターの種別(定数バッファビュー)
          descriptorRanges[0].NumDescriptors = 1;															// ディスクリプターの数
          descriptorRanges[0].BaseShaderRegister = 0;														// 範囲内のベースシェーダレジスタ
          descriptorRanges[0].RegisterSpace = 0;															// レジスタ空間(TODO:調べる)
          descriptorRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// ルートシグネチャ開始からのディスクリプタのオフセット?

          // StructuredBuffer用ディスクリプターレンジの設定
          descriptorRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;								// ディスクリプターの種別(シェーダーリソースビュー)
          descriptorRanges[1].NumDescriptors = 1;															// ディスクリプターの数
          descriptorRanges[1].BaseShaderRegister = 0;														// 範囲内のベースシェーダレジスタ
          descriptorRanges[1].RegisterSpace = 0;															// レジスタ空間(TODO:調べる)
          descriptorRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;	// ルートシグネチャ開始からのディスクリプタのオフセット?

          // テクスチャ用ディスクリプターレンジの設定
          descriptorRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;                              // ディスクリプターの種別(シェーダーリソースビュー)
          descriptorRanges[2].NumDescriptors = 3;                                                       // ディスクリプターの数
          descriptorRanges[2].BaseShaderRegister = 0;                                                   // 範囲内のベースシェーダレジスタ
          descriptorRanges[2].RegisterSpace = 0;                                                        // レジスタ空間(TODO:調べる)
          descriptorRanges[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // ルートシグネチャ開始からのディスクリプタのオフセット?
        }

        // ルートパラメータの設定
        D3D12_ROOT_PARAMETER rootParameters[3];
        {
          // 定数バッファ用ルートパラメータ
          rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ルートシグネチャのスロットの種別(ディスクリプタテーブル)
          rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;									// ディスクリプターレンジの数
          rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRanges[0];					// ディスクリプターレンジのポインタ(数が1超なら配列の先頭ポインタ)
          rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;						// ルートシグネチャのスロットの内容にアクセスできるシェーダの種別(頂点シェーダのみ)

          // StructuredBuffer用ルートパラメータ
          rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ルートシグネチャのスロットの種別(ディスクリプタテーブル)
          rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;									// ディスクリプターレンジの数
          rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRanges[1];					// ディスクリプターレンジのポインタ(数が1超なら配列の先頭ポインタ)
          rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;						// ルートシグネチャのスロットの内容にアクセスできるシェーダの種別(頂点シェーダのみ)

          // テクスチャ用ルートパラメータ
          rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;				// ルートシグネチャのスロットの種別(ディスクリプタテーブル)
          rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;									// ディスクリプターレンジの数
          rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRanges[2];					// ディスクリプターレンジのポインタ(数が1超なら配列の先頭ポインタ)
          rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;							// ルートシグネチャのスロットの内容にアクセスできるシェーダの種別(ピクセルシェーダのみ)
        }

        // サンプラーの設定
        D3D12_STATIC_SAMPLER_DESC sampler = {};
        sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;					// フィルタリング方法(ポイントサンプリング)
        sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// 範囲外にあるU座標の解決方法(境界線の色を使用)
        sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// 範囲外にあるV座標の解決方法(境界線の色を使用)
        sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// 範囲外にあるW座標の解決方法(境界線の色を使用)
        sampler.MipLODBias = 0;												// ミップマップ レベルからのオフセット
        sampler.MaxAnisotropy = 0;											// D3D12_FILTER_ANISOTROPICかD3D12_FILTER_COMPARISON_ANISOTROPICをフィルタとして指定した場合のクランプ値
        sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;				// 既存のサンプリングデータに対してデータを比較する関数(比較しない)
        sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// 範囲外座標の解決方法に対してD3D12_TEXTURE_ADDRESS_MODE_BORDERが指定されている場合に使用される境界の色
        sampler.MinLOD = 0.0f;												// クランプするミップマップ範囲の下限
        sampler.MaxLOD = D3D12_FLOAT32_MAX;									// クランプするミップマップ範囲の上限(上限がない場合はD3D12_FLOAT32_MAX等の大きいサイズを指定する)
        sampler.ShaderRegister = 0;											// レジスター番号(「register(s2, space3)」のs2の番号指定用)
        sampler.RegisterSpace = 0;											// レジスタースペース(「register(s2, space3)」のspace3の番号指定用)
        sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// ピクセルシェーダーからアクセス可能

        // ルートシグネチャの設定
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.NumParameters = 3;													// ルートシグネチャのスロット数
        rootSignatureDesc.pParameters = rootParameters;											// スロットの構造?
        rootSignatureDesc.NumStaticSamplers = 1;												// 静的サンプラー数
        rootSignatureDesc.pStaticSamplers = &sampler;											// 静的サンプラー設定データのポインタ
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT	// オプション(描画に使用する)
          | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS							// ハルシェーダからルートシグネチャへのアクセス禁止
          | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS							// ドメインシェーダからルートシグネチャへのアクセス禁止
          | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;						// ジオメトリシェーダからルートシグネチャへのアクセス禁止


        // ルートシグネチャのシリアル化
        Microsoft::WRL::ComPtr<ID3DBlob> signature;
        if (FAILED(D3D12SerializeRootSignature(
          &rootSignatureDesc,				// ルートシグネチャの設定
          D3D_ROOT_SIGNATURE_VERSION_1,	// ルートシグネチャのバージョン
          &signature,						// シリアライズしたルートシグネチャへアクセスするためのインターフェイス(ポインタ)
          nullptr							// シリアライザのエラーメッセージへアクセスするためのインターフェイス(ポインタ)
        )))
        {
          throw "ルートシグネチャのシリアライズに失敗しました。";
        }

        // ルートシグネチャの生成
        if (FAILED(device->CreateRootSignature(
          0,									// マルチアダプター(マルチGPU)の場合に使用するアダプター(GPU)の識別子(単一なので0)
          signature->GetBufferPointer(),		// シリアル化されたシグネチャ設定へのポインタ
          signature->GetBufferSize(),			// メモリのブロックサイズ
          IID_PPV_ARGS(&rootSignature))))
        {
          throw "ルートシグネチャの生成に失敗しました。";
        }
      }

      // パイプラインステートの作成
      // シェーダーも一緒にコンパイルする
      // 後々はコンパイル済みのシェーダーを使用する
      {
        Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
        Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

        // コンパイルオプションフラグを設定する
#if defined(_DEBUG)
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        // 頂点シェーダーのコンパイル
        if (FAILED(D3DCompileFromFile(
          L"shaders.hlsl",	// シェーダーファイル名
          nullptr,			// シェーダーマクロ(今回は使用しない)
          nullptr,			// インクルードファイルを取り扱うために使用するID3DIncludeインタフェースへのポインタ(今回は使用しない)
          "VSMain",			// エントリーポイントの関数名
          "vs_5_0",			// コンパイルターゲット(今回は頂点シェーダーでシェーダーモデル5)
          compileFlags,		// コンパイルオプション
          0,					// エフェクトファイルのコンパイルオプション(今回はエフェクトとして使用しないので0)
          &vertexShader,		// コンパイルされたコードへアクセスするためのID3DBlobインタフェースのポインタ
          nullptr				// コンパイルエラーメッセージへアクセスするためのID3DBlobインタフェースのポインタ
        )))
        {
          throw "頂点シェーダーのコンパイルに失敗しました。";
        }

        // ピクセルシェーダーのコンパイル
        if (FAILED(D3DCompileFromFile(
          L"shaders.hlsl",	// シェーダーファイル名
          nullptr,			// シェーダーマクロ(今回は使用しない)
          nullptr,			// インクルードファイルを取り扱うために使用するID3DIncludeインタフェースへのポインタ(今回は使用しない)
          "PSMain",			// エントリーポイントの関数名
          "ps_5_0",			// コンパイルターゲット(今回はピクセルシェーダーでシェーダーモデル5)
          compileFlags,		// コンパイルオプション
          0,					// エフェクトファイルのコンパイルオプション(今回はエフェクトとして使用しないので0)
          &pixelShader,		// コンパイルされたコードへアクセスするためのID3DBlobインタフェースのポインタ
          nullptr				// コンパイルエラーメッセージへアクセスするためのID3DBlobインタフェースのポインタ
        )))
        {
          throw "ピクセルシェーダーのコンパイルに失敗しました。";
        }

        // 頂点入力レイアウト
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
          // 頂点データ
          { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
          { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
          { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        // ラスタライザーステートの設定
        D3D12_RASTERIZER_DESC rasterizer_desc;
        rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;                               // 三角形を描画するときに使用する塗りつぶしモード(今回は塗りつぶし)
        rasterizer_desc.CullMode = D3D12_CULL_MODE_NONE;                                // カリングのモード(裏向きのポリゴンを描画しない)
        rasterizer_desc.FrontCounterClockwise = FALSE;                                  // ポリゴンの表裏の判定方法(今回は時計回りなら表)
        rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // 深度バイアス(Z-Fighting対策?)
        rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // 深度バイアスのクランプ値
        rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // 傾斜を考慮した深度バイアス(ピーターパン現象対策?)
        rasterizer_desc.DepthClipEnable = TRUE;                                         // Zクリッピングを行うか(今回は行う)
        rasterizer_desc.MultisampleEnable = FALSE;                                      // マルチサンプリングのアンチエイリアシングを行うか(今回はしない)
        rasterizer_desc.AntialiasedLineEnable = FALSE;                                  // 線のアンチエイリアシングを行うか(今回はしない)
        rasterizer_desc.ForcedSampleCount = 0;                                          // UAVレンダリングまたはラスタライズ中に強制されるサンプル数(今回は強制しない)
        rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 保守的なラスタライズフラグ(今回はオフ)

        // パイプラインステートの設定に使用する頂点シェーダーのデータ構造を作成
        D3D12_SHADER_BYTECODE vs;
        vs.pShaderBytecode = vertexShader.Get()->GetBufferPointer();
        vs.BytecodeLength = vertexShader.Get()->GetBufferSize();

        // パイプラインステートの設定に使用するピクセルシェーダーのデータ構造を作成
        D3D12_SHADER_BYTECODE ps;
        ps.pShaderBytecode = pixelShader.Get()->GetBufferPointer();
        ps.BytecodeLength = pixelShader.Get()->GetBufferSize();

        // レンダーターゲットのブレンド状態の設定
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTarggetBlendDesc =
        {
          false,                        // ブレンディングを有効にするか(今回は無効)
          false,                        // 論理演算を有効にするか(今回は無効)
          D3D12_BLEND_ONE,              // RGB値ピクセルシェーダ出力に対して実行する操作を指定
          D3D12_BLEND_ZERO,             // レンダーターゲットの現在のRGB値に対して実行する動作を指定する
          D3D12_BLEND_OP_ADD,           // RGBまたはアルファブレンディング操作を指定(ソース1とソース2を追加)
          D3D12_BLEND_ONE,              // ピクセルシェーダ出力するアルファ値に対して実行する動作を指定
          D3D12_BLEND_ZERO,             // レンダーターゲットで現在のアルファ値に対して実行する動作
          D3D12_BLEND_OP_ADD,           // RBGまたはアルファブレンディング操作を指定(ソース1とソース2を追加)
          D3D12_LOGIC_OP_NOOP,          // レンダーターゲットに設定する論理演算?(ノーオペレーション、何もしない)
          D3D12_COLOR_WRITE_ENABLE_ALL  // 
        };

        // ブレンド状態の設定
        D3D12_BLEND_DESC blendDesc;
        blendDesc.AlphaToCoverageEnable = false;  // アルファトゥカバレッジを有効にするか
        blendDesc.IndependentBlendEnable = false; // 同時レンダーターゲットに独立したブレンドを有効にするかどうかを指定します
        for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
          blendDesc.RenderTarget[i] = defaultRenderTarggetBlendDesc;

        // 深度ステンシル状態の設定
        D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = true;                          // デプステストを有効にするか?
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 深度ステンシル バッファーへの書き込みをオンにします
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;      // 深度データを既存の深度データと比較する関数です。ソースデータが対象データよりも小さい場合、比較に合格します。
        depthStencilDesc.StencilEnable = false;                       // ステンシルテストを有効にするか?

        // マルチサンプリングパラメーターの設定
        DXGI_SAMPLE_DESC sampleDesc = {};
        sampleDesc.Count = 1; // ピクセル単位のマルチサンプリングの数

        // パイプラインステートの設定
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) }; // 入力レイアウトの構造
        psoDesc.pRootSignature = rootSignature;                                   // ルートシグネチャ
        psoDesc.RasterizerState = rasterizer_desc;                                // ラスタライザの状態
        psoDesc.VS = vs;                                                          // 頂点シェーダーの構造
        psoDesc.PS = ps;                                                          // ピクセルシェーダーの構造
        psoDesc.BlendState = blendDesc;                                           // ブレンド状態の構造
        psoDesc.DepthStencilState = depthStencilDesc;                             // 深度ステンシル状態の構造
        psoDesc.SampleMask = UINT_MAX;                                            // ブレンドの状態のためのサンプルのマスク
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // 入力プリミティブ(三角形)
        psoDesc.NumRenderTargets = 1;                                             // レンダーターゲットのフォーマット数
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                       // レンダーターゲットのフォーマット
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                // 深度ステンシルのフォーマット
        psoDesc.SampleDesc = sampleDesc;                                          // サンプリング状態の構造

        // グラフィックスパイプラインステートの生成
        if (FAILED(device->CreateGraphicsPipelineState(
          &psoDesc,
          IID_PPV_ARGS(&pipelineState))))
        {
          ID3D12DebugDevice* debugInterface;
          if (SUCCEEDED(device->QueryInterface(&debugInterface)))
          {
            debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
            debugInterface->Release();
          }

          throw "パイプラインステートの生成に失敗しました。";
        }
      }
    }

    /**
     *  @brief  描画する
     *  @param  vertexBuffer:頂点バッファ
     *  @param  indexBuffer:頂点インデックスバッファ
     *  @param  indexCount:頂点インデックス数
     *  @param  instanceCount:インスタンス数
     */
    void Device::Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer, const unsigned int indexCount, const unsigned int instanceCount)
    {
      // ビューポートの作成
      D3D12_VIEWPORT viewport;
      viewport.TopLeftX = 0;
      viewport.TopLeftY = 0;
      viewport.Width = 600;
      viewport.Height = 400;
      viewport.MinDepth = 0;
      viewport.MaxDepth = 1;

      // シザー矩形(シザーテスト)の作成
      D3D12_RECT scissor;
      scissor.left = 0;
      scissor.top = 0;
      scissor.right = 600;
      scissor.bottom = 400;

      // パイプラインステートの設定(切り替えない場合は、コマンドリストリセット時に設定可能)
      commandList->Get().SetPipelineState(pipelineState);

      // グラフィックスパイプラインのルートシグネチャを設定する
      commandList->Get().SetGraphicsRootSignature(rootSignature);

      // 描画に使用するディスクリプターヒープを設定
      auto cbvSrvHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Get();
      commandList->Get().SetDescriptorHeaps(1, &cbvSrvHeap);

      // ビューポートの設定
      commandList->Get().RSSetViewports(1, &viewport);

      // シザー矩形(シザーテスト)の設定
      commandList->Get().RSSetScissorRects(1, &scissor);

      // プリミティブトポロジーの設定
      //commandList->Get().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
      commandList->Get().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      // 頂点バッファビューの設定
      commandList->Get().IASetVertexBuffers(0, 1, &(vertebBuffer.GetView()));

      // 頂点インデックスビューの設定
      commandList->Get().IASetIndexBuffer(&(indexBuffer.GetView()));

      // ディスクリプータヒープテーブルを設定
      auto handleCbv = cbvSrvHeap->GetGPUDescriptorHandleForHeapStart();  // 定数バッファ用ディスクリプータヒープテーブル
      auto handleSrv = cbvSrvHeap->GetGPUDescriptorHandleForHeapStart();  // StructuredBuffer用ディスクリプータヒープテーブル
      auto handleTrv = cbvSrvHeap->GetGPUDescriptorHandleForHeapStart();  // テクスチャ用ディスクリプータヒープテーブル
      handleSrv.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
      handleTrv.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 2;
      commandList->Get().SetGraphicsRootDescriptorTable(0, handleCbv);
      commandList->Get().SetGraphicsRootDescriptorTable(1, handleSrv);
      commandList->Get().SetGraphicsRootDescriptorTable(2, handleTrv);

      // 描画コマンドの生成
      commandList->Get().DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
    }

    /**
     *  @brief  定数バッファを作成する
     *  @param  size:定数バッファのサイズ
     *  @return 定数バッファへのポインタ
     */
    std::unique_ptr<ConstantBuffer> Device::CreateConstantBuffer(const unsigned int size)
    {
      auto constantBuffer = std::make_unique<ConstantBuffer>();
      auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
      auto& descriptor = descriptorHeap.CreateDescriptor();
      constantBuffer->Create(device.get(), descriptor.GetHandleForCPU(), size);

      return constantBuffer;
    }

    /**
     *  @brief  シェーダーリソースバッファを作成する
     *  @param  num:リソース内の要素数
     *  @param  size:リソース内の1要素のサイズ
     *  @return シェーダーリソースバッファへのポインタ
     */
    std::unique_ptr<ShaderResourceBuffer> Device::CreateShaderResourceBuffer(const unsigned int num, const unsigned int size)
    {
      // ヒープの設定
      D3D12_HEAP_PROPERTIES properties;
      properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
      properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPUページプロパティ(不明に設定)
      properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // ヒープのメモリプール(不明に設定)
      properties.CreationNodeMask = 1;                              // 恐らくヒープが生成されるアダプター(GPU)の番号
      properties.VisibleNodeMask = 1;                               // 恐らくヒープが表示されるアダプター(GPU)の番号

      auto shaderResourceBuffer = std::make_unique<ShaderResourceBuffer>();
      auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
      auto& descriptor = descriptorHeap.CreateDescriptor();
      shaderResourceBuffer->Create(device.get(), descriptor.GetHandleForCPU(), num, size);

      return shaderResourceBuffer;
    }

    /**
     *  @brief  デバイスを取得する
     *  @return デバイスへの参照
     */
    ID3D12Device& Device::GetDevice() const
    {
      return *device;
    }

    // 後々別クラスへ移動する
#pragma region Texture
    /**
     *  @brief  テクスチャバッファを生成する
     *  @param  data:テクスチャデータ
     *  @param  width:横幅
     *  @param  height:縦幅
     *  @param  bytesPerPixel:1ピクセルで使用するバイト数
     */
    void Device::CreateTextureBuffer(const uint8_t* const data, const uint32_t width, const uint32_t height, const uint8_t bytesPerPixel)
    {
      // リソースの作成
      {
        D3D12_HEAP_PROPERTIES properties;
        properties.Type = D3D12_HEAP_TYPE_DEFAULT;
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        properties.CreationNodeMask = 1;
        properties.VisibleNodeMask = 1;

        Texture* texture = new Texture;
        texture->Create(device.get(), properties, width, height);
        texBuffer.reset(texture);
      }

      // 中間リソースの作成
      {
        std::unique_ptr<Sein::Direct3D12::Buffer> buffer = std::make_unique<Buffer>();

        D3D12_RESOURCE_DESC Desc = texBuffer->Get().GetDesc();
        UINT64 RequiredSize = 0;

        device->GetCopyableFootprints(&Desc, 0, 1, 0, nullptr, nullptr, nullptr, &RequiredSize);

        D3D12_HEAP_PROPERTIES properties;
        properties.Type = D3D12_HEAP_TYPE_UPLOAD;
        properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        properties.CreationNodeMask = 1;
        properties.VisibleNodeMask = 1;
        buffer->Create(device.get(), properties, RequiredSize);

        // データのコピー
        {
          D3D12_SUBRESOURCE_DATA textureData = {};
          textureData.pData = data;
          textureData.RowPitch = width * bytesPerPixel;
          textureData.SlicePitch = textureData.RowPitch * height;

          unsigned char* pData;

          if (FAILED(buffer->Get().Map(0, nullptr, reinterpret_cast<void**>(&pData))))
          {
            throw "テクスチャ用中間リソースへのポインタの取得に失敗しました。";
          }
          std::memcpy(pData, data, sizeof(uint8_t) * textureData.SlicePitch);
          buffer->Get().Unmap(0, nullptr);
        }

        // テクスチャ用リソースへコピー
        {
          commandList->Begin();

          D3D12_RESOURCE_DESC Desc = texBuffer->Get().GetDesc();
          D3D12_PLACED_SUBRESOURCE_FOOTPRINT footPrint;
          device->GetCopyableFootprints(&Desc, 0, 1, 0, &footPrint, nullptr, nullptr, nullptr);

          D3D12_TEXTURE_COPY_LOCATION Dst;
          Dst.pResource = &(texBuffer->Get());
          Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
          Dst.SubresourceIndex = 0;

          D3D12_TEXTURE_COPY_LOCATION Src;
          Src.pResource = &(buffer->Get());
          Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
          Src.PlacedFootprint = footPrint;
          commandList->Get().CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);

          // リソースバリア
          D3D12_RESOURCE_BARRIER barrier;
          barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
          barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
          barrier.Transition.pResource = &(texBuffer->Get());
          barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
          barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
          barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
          commandList->Get().ResourceBarrier(1, &barrier);

          // コマンドリストをクローズする
          // コマンドリストの実行
          commandList->End();
          ID3D12CommandList* ppCommandLists[] = { &(commandList->Get()) };
          commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

          WaitForGpu();
        }
      }

      // シェーダーリソースビューの作成
      {
        // Describe and create a SRV for the texture.
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        auto& descriptorHeap = descriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
        auto& descriptor = descriptorHeap.CreateDescriptor();
        device->CreateShaderResourceView(&(texBuffer->Get()), &srvDesc, descriptor.GetHandleForCPU());
      }
    }
#pragma endregion
  };
};