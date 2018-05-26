/**
 *  @file     direct3d12_device.cc
 *  @brief    Direct3D12デバイスに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/02/07
 *  @version  1.0
 */

#include "direct3d12_device.h"

#include <d3d12.h>
#include <wrl\client.h>
#include <dxgi1_4.h>

#include "DirectXTK12/include/ResourceUploadBatch.h"

#include "fence.h"
#include "texture_view.h"
#include "depth_stencil_view.h"
#include "descriptor_range.h"
#include "root_parameter.h"
#include "root_signature.h"
#include "rasterizer_desc.h"
#include "graphics_pipeline_state.h"
#include "command_queue.h"
#include "swap_chain.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      constexpr std::uint32_t FRAME_COUNT = 2;

      /**
       *  @brief  デバイス用クラス
       */
      class Device final : public IDevice
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Device() : device_(nullptr, [](ID3D12Device* p) { if (p) { p->Release(); } }), command_queue_(nullptr), swap_chain_(nullptr),
          root_signature_(nullptr), pipeline_state_(nullptr), render_target_list_(),
          descriptor_heaps_(), depthStencilView(nullptr), fence(nullptr), texBuffer()
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Device() override
        {
          Release();
        }

        /**
         *  @brief  生成する
         *  @param  handle:ウィンドウハンドル
         *  @param  width:横幅
         *  @param  height:縦幅
         */
        void Create(const HWND handle, const std::uint32_t width, const std::uint32_t height)
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
              throw std::exception("デバイスの生成に失敗しました。");
            }
            device_.reset(pDevice);
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
          // コマンドキューの作成
          // コマンドキューはGPUへ描画命令が出されたコマンドリストを
          // 順次実行する
          // つまり特定のアダプター(GPU)に関連付けられている
          {
            D3D12_COMMAND_QUEUE_DESC command_queue_desc = {};
            command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;  // タイムアウト処理を有効にする
            command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;  // レンダリング関連のコマンドリスト
            command_queue_ = this->CreateCommandQueue(command_queue_desc);
          }

          // スワップチェインの作成
          // コマンドキューを指定して作成する = アダプターを指定して作成する
          {
            DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
            swap_chain_desc.Width = width;                                  // ウィンドウ横幅
            swap_chain_desc.Height = height;                                // ウィンドウ縦幅
            swap_chain_desc.BufferCount = FRAME_COUNT;                      // バッファの数は2個(フロントバッファも含むらしいが、公式サンプル等を見るとバックバッファの数な気がする)
            swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // 恐らくバッファのフォーマット(4 成分、32 ビット符号なし整数)
            swap_chain_desc.Scaling = DXGI_SCALING_NONE;                    // 画面サイズとバッファサイズが等しくない時の拡縮動作(拡大縮小は行わない)
            swap_chain_desc.SampleDesc.Quality = 0;                         // マルチサンプリングの品質レベル
            swap_chain_desc.SampleDesc.Count = 1;                           // ピクセル単位のマルチサンプリング数
            swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // バックバッファの使用目的及びCPUアクセスオプション(レンダーターゲットとして使用)
            swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // スワップチェインの動作オプション(モード切替可能に設定)
            swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // フロントバッファとバックバッファのスワップ挙動指定(バックバッファがディスプレイに表示されたら破棄する)
            swap_chain_ = this->CreateSwapChain(factory.Get(), handle, swap_chain_desc);
          }

          // Alt + Enterでフルスクリーン化の機能を無効に設定
          factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);

          // ディスクリプターヒープ用メモリ領域の確保
          {
            descriptor_heaps_.resize(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES);
          }

          // 定数バッファビュー、シェーダーリソースビュー用ディスクリプターヒープを生成
          {
            D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
            cbvHeapDesc.NumDescriptors = 5;                                 // ディスクリプターヒープ内のディスクリプター数(定数バッファ、シェーダーリソース)
            cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      // 定数バッファ or シェーダーリソース(テクスチャ) or ランダムアクセス のどれかのヒープ
            cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // シェーダーからアクセス可

            descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV] = this->CreateDescriptorHeap(cbvHeapDesc);
          }

          // 深度ステンシルビュー用ディスクリプターヒープを生成
          {
            D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
            heapDesc.NumDescriptors = 1;                      // ディスクリプターヒープ内のディスクリプター数(深度ステンシルビュー)
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;   // 深度ステンシルビューのヒープ
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // シェーダーからアクセス不可
            
            descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_DSV] = this->CreateDescriptorHeap(heapDesc);
          }

          // レンダーターゲット
          {
            // レンダーターゲットビュー用ディスクリプターヒープの作成
            // ディスクリプターはバッファの情報データ(テクスチャバッファ、頂点バッファ等)
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FRAME_COUNT;             // ディスクリプターヒープ内のディスクリプター数(フロントバッファ、バックバッファ)
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;    // 種別はレンダーターゲットビュー
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // シェーダーから参照しない

            descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV] = this->CreateDescriptorHeap(rtvHeapDesc);

            // ディスクリプターの登録
            {
              // フレームバッファ数だけ登録する
              render_target_list_.resize(FRAME_COUNT);
              for (auto i = 0; i < FRAME_COUNT; ++i)
              {
                decltype(auto) render_target = render_target_list_[i].get();
                if (FAILED(swap_chain_->GetBuffer(i, &render_target)))
                {
                  throw std::exception("バックバッファの取得に失敗しました。");
                }
                render_target_list_[i].reset(render_target);

                const auto& descriptor = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV]->CreateDescriptor();

                // レンダーターゲットビュー用のディスクリプターを作成する
                // ディスクリプターヒープの領域に作成される
                device_->CreateRenderTargetView(
                  render_target,        // レンダー ターゲットを表すID3D12Resourceへのポインタ
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
            fence->Create(device_.get());

            // 描画処理を行っている可能性があるので描画終了待ちを行う
            WaitForGpu();
          }

          // アセットを読み込む
          {
            LoadAssets(width, height);
          }
        }

        /**
         *  @brief  リソースを開放する
         */
        void Release() noexcept override
        {
          // GPUの描画終了待ちを行う
          WaitForGpu();

          for (auto& render_target : render_target_list_)
          {
            render_target->Release();
            render_target.release();
          }
        }

        /**
         *  @brief  シーンを開始する
         */
        void BeginScene(ICommandList* const command_list, std::uint32_t buffer_index) override
        {
          decltype(auto) render_target = render_target_list_[buffer_index];
          decltype(auto) descriptor_heap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV];
          const auto& descriptor_for_rtv = descriptor_heap->GetDescriptor(buffer_index);

          // TODO:const_castの削除
          decltype(auto) graphics_command_list = const_cast<ID3D12GraphicsCommandList&>(command_list->Get());

          command_list->Begin();

          // バックバッファが描画ターゲットとして使用できるようになるまで待つ
          // リソースは描画ターゲット, 遷移前はPresent, 遷移後は描画ターゲット
          command_list->ResourceBarrier(render_target.get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

          // バックバッファを描画ターゲットとして設定する
          // デバイスへ深度ステンシルビューをバインドする

          auto& dsvDescriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
          const auto& dsvDescriptor = dsvDescriptorHeap->GetDescriptor(0); // TODO:マジックナンバーを消す
          graphics_command_list.OMSetRenderTargets(1, &descriptor_for_rtv.GetHandleForCPU(), false, &dsvDescriptor.GetHandleForCPU());

          // バックバッファをクリアする
          const float Color[] = { 0.0f, 0.0f, 0.6f, 1.0f };
          graphics_command_list.ClearRenderTargetView(descriptor_for_rtv.GetHandleForCPU(), Color, 0, nullptr);

          // 深度ステンシルビューをクリアする(深度バッファのみ)
          graphics_command_list.ClearDepthStencilView(dsvDescriptor.GetHandleForCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
        }
        
        /**
         *  @brief  描画する
         *  @param  command_list:コマンドリスト
         *  @param  indexCount:頂点インデックス数
         *  @param  instanceCount:インスタンス数
         */
        void Render(ICommandList* const command_list, const unsigned int indexCount, const unsigned int instanceCount) override
        {
          // TODO:const_castの削除
          decltype(auto) graphics_command_list = const_cast<ID3D12GraphicsCommandList&>(command_list->Get());

          // ビューポートの作成
          D3D12_VIEWPORT viewport;
          viewport.TopLeftX = 0;
          viewport.TopLeftY = 0;
          viewport.Width = 600;
          viewport.Height = 400;
          viewport.MinDepth = 0;
          viewport.MaxDepth = 1;

          // ビューポートの設定
          command_list->SetViewports(1, &viewport);

          // シザー矩形(シザーテスト)の作成
          D3D12_RECT scissor;
          scissor.left = 0;
          scissor.top = 0;
          scissor.right = 600;
          scissor.bottom = 400;

          // シザー矩形(シザーテスト)の設定
          command_list->SetScissorRects(1, &scissor);

          // パイプラインステートの設定(切り替えない場合は、コマンドリストリセット時に設定可能)
          pipeline_state_->SetPipelineState(&graphics_command_list);

          // グラフィックスパイプラインのルートシグネチャを設定する
          root_signature_->SetGraphicsRootSignature(&graphics_command_list);

          // 描画に使用するディスクリプターヒープを設定
          auto& descriptor_heap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
          descriptor_heap->SetDescriptorHeaps(&graphics_command_list);

          // ディスクリプータヒープテーブルを設定
          // TODO:ディスクリプターテーブルの設定をディスクリプターヒープに移動する
          graphics_command_list.SetGraphicsRootDescriptorTable(0, descriptor_heap->GetDescriptor(0).GetHandleForGPU());  // 定数バッファ用ディスクリプータヒープテーブル
          graphics_command_list.SetGraphicsRootDescriptorTable(1, descriptor_heap->GetDescriptor(1).GetHandleForGPU());  // StructuredBuffer用ディスクリプータヒープテーブル
          //graphics_command_list.SetGraphicsRootDescriptorTable(2, descriptor_heap->GetDescriptor(2).GetHandleForGPU());  // テクスチャ用ディスクリプータヒープテーブル

          // 描画コマンドの生成
          graphics_command_list.DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
        }
        
        /**
         *  @brief  シーンを終了する
         */
        void EndScene(ICommandList* const command_list, std::uint32_t buffer_index) override
        {
          decltype(auto) render_target = render_target_list_[buffer_index];

          // バックバッファの描画完了を待つためのバリアを設置
          // リソースは描画ターゲット, 遷移前は描画ターゲット, 遷移後はPresent
          command_list->ResourceBarrier(render_target.get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

          // コマンドリストをクローズする
          command_list->End();
        }
        
        /**
         *  @brief  画面を更新する
         */
        void Present() override
        {
          // 描画終了待ちを行う
          WaitForGpu();

          // 画面の更新
          swap_chain_->Present(1, 0);
        }
        
        /**
         *  @brief  定数バッファを作成する
         *  @param  size_in_bytes:定数バッファのサイズ
         *  @return 定数バッファのユニークポインタ
         */
        std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const std::uint32_t size_in_bytes) override
        {
          auto& descriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
          return this->CreateConstantBuffer(descriptorHeap, size_in_bytes);
        }

        /**
         *  @brief  定数バッファを作成する
         *  @param  descriptor_heap:定数バッファを作成するディスクリプターヒープ
         *  @param  size_in_bytes:定数バッファのサイズ
         *  @return 定数バッファのユニークポインタ
         */
        std::unique_ptr<IConstantBuffer> CreateConstantBuffer(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t size_in_bytes) override
        {
          auto& descriptor = descriptor_heap->CreateDescriptor();
          return IConstantBuffer::Create(device_.get(), descriptor.GetHandleForCPU(), size_in_bytes);
        }
        
        /**
         *  @brief  シェーダーリソースバッファを作成する
         *  @param  num:リソース内の要素数
         *  @param  size:リソース内の1要素のサイズ
         *  @return シェーダーリソースバッファのユニークポインタ
         */
        std::unique_ptr<ShaderResourceBuffer> CreateShaderResourceBuffer(const unsigned int num, const unsigned int size) override
        {
          // ヒープの設定
          D3D12_HEAP_PROPERTIES properties;
          properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // ヒープの種類(今回はCPU、GPUからアクセス可能なヒープに設定)
          properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPUページプロパティ(不明に設定)
          properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // ヒープのメモリプール(不明に設定)
          properties.CreationNodeMask = 1;                              // 恐らくヒープが生成されるアダプター(GPU)の番号
          properties.VisibleNodeMask = 1;                               // 恐らくヒープが表示されるアダプター(GPU)の番号

          auto shaderResourceBuffer = std::make_unique<ShaderResourceBuffer>();
          auto& descriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
          auto& descriptor = descriptorHeap->CreateDescriptor();
          shaderResourceBuffer->Create(device_.get(), descriptor.GetHandleForCPU(), num, size);

          return shaderResourceBuffer;
        }
        
        /**
         *  @brief  コマンドリストを作成する
         *  @param  command_list_type:コマンドリストのタイプ
         *  @return コマンドリストのシェアードポインタ
         */
        std::shared_ptr<ICommandList> CreateCommandList(const D3D12_COMMAND_LIST_TYPE& command_list_type) override
        {
          return ICommandList::Create(device_.get(), command_list_type);
        }
        
        /**
         *  @brief  頂点バッファを作成する
         *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
         *  @return 頂点バッファへのユニークポインタ
         */
        std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) override
        {
          return IVertexBuffer::Create(device_.get(), size_in_bytes);
        }
        
        /**
         *  @brief  頂点インデックスバッファを作成する
         *  @param  size_in_bytes:頂点インデックスバッファのサイズ(頂点インデックスサイズ * 頂点インデックス数)
         *  @return 頂点インデックスバッファへのユニークポインタ
         */
        std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) override
        {
          return IIndexBuffer::Create(device_.get(), size_in_bytes);
        }
        
        /**
         *  @brief  シェーダーを作成する
         *  @param  shader_file_path:シェーダーファイルのパス
         *  @return シェーダーへのシェアードポインタ
         */
        std::shared_ptr<IShader> CreateShader(const std::string& shader_file_path) override
        {
          return IShader::Create(shader_file_path);
        }
        
        /**
         *  @brief  次にバックバッファになるバッファの番号を取得する
         *  @return 次にバックバッファになるバッファの番号
         */
        const std::uint32_t GetNextBackBufferIndex() const noexcept override
        {
          return (swap_chain_->GetCurrentBackBufferIndex() + 1) % render_target_list_.size();
        }
        
        /**
         *  @brief  ファイルからテクスチャバッファを生成する
         *  @param  file_path:テクスチャファイルパス
         */
        void CreateTextureBufferFromFile(const std::wstring& file_path) override
        {
          DirectX::ResourceUploadBatch upload_batch(device_.get());
          upload_batch.Begin();

          decltype(auto) texture = ITexture::CreateFromFile(device_.get(), upload_batch, file_path);

          // TODO:const_castを削除する
          auto uploadResourcesFinished = upload_batch.End(const_cast<ID3D12CommandQueue*>(&(command_queue_->Get())));

          WaitForGpu();

          uploadResourcesFinished.wait();

          texBuffer.emplace_back(std::make_unique<TextureView>());
          auto texture_view = texBuffer[texBuffer.size() - 1].get();

          texture_view->Create(device_.get(), texture.release(), descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].get());
        }
        
        /**
         *  @brief  コマンドリストを実行する
         *  @param  command_list:コマンドリスト
         */
        void ExecuteCommandLists(ICommandList* const command_list) override
        {
          // TODO:const_castの削除
          decltype(auto) graphics_command_list = const_cast<ID3D12GraphicsCommandList&>(command_list->Get());

          ID3D12CommandList* ppCommandLists[] = { &graphics_command_list };
          command_queue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
        }

        /**
         *  @brief  ディスクリプターヒープを生成する
         *  @param  descriptor_heap_desc:ディスクリプターヒープの設定
         *  @return ディスクリプターヒープのシェアードポインタ
         */
        std::shared_ptr<IDescriptorHeap> CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& descriptor_heap_desc) override
        {
          return IDescriptorHeap::Create(device_.get(), descriptor_heap_desc);
        }

      private:
        /**
         *  @brief  アセットを読み込む
         *  @param  width:ウィンドウ横幅
         *  @param  height:ウィンドウ縦幅
         */
        void LoadAssets(const std::uint32_t width, const std::uint32_t height)
        {
          // 深度ステンシルビューの作成
          {
            depthStencilView = std::make_unique<DepthStencilView>();
            auto& descriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
            auto& descriptor = descriptorHeap->CreateDescriptor();
            depthStencilView->Create(device_.get(), descriptor.GetHandleForCPU(), width, height);
          }

          // ルートシグネチャの作成
          {
            // 定数バッファのディスクリプターレンジを作成
            auto descriptor_range_for_cbv = IDescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1);

            // StructuredBuffer(シェーダーリソース)のディスクリプターレンジを作成
            auto descriptor_range_for_sbv = IDescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1);

            // テクスチャ(シェーダーリソース)のディスクリプターレンジを作成
            auto descriptor_range_for_texture = IDescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3);

            // ルートパラメータの設定
            D3D12_ROOT_PARAMETER rootParameters[3];

            // 定数バッファ用ルートパラメータ
            auto root_parameter_for_cbv = IRootParameter::CreateForDescriptorTable(*descriptor_range_for_cbv, D3D12_SHADER_VISIBILITY_VERTEX);
            rootParameters[0] = root_parameter_for_cbv->Get();

            // StructuredBuffer用ルートパラメータ
            auto root_parameter_for_sbv = IRootParameter::CreateForDescriptorTable(*descriptor_range_for_sbv, D3D12_SHADER_VISIBILITY_VERTEX);
            rootParameters[1] = root_parameter_for_sbv->Get();

            // テクスチャ用ルートパラメータ
            auto root_paramter_for_texture = IRootParameter::CreateForDescriptorTable(*descriptor_range_for_texture, D3D12_SHADER_VISIBILITY_PIXEL);
            rootParameters[2] = root_paramter_for_texture->Get();


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

                                                                                      // ルートシグネチャの作成
            root_signature_ = IRootSignature::Create(device_.get(), rootSignatureDesc);
          }

          // パイプラインステートの作成
          {
            // シェーダーファイルの読み込み
            // TODO:相対パスを指定できるように
            auto vertex_shader = this->CreateShader("D:/DiskD/Study/Multithread/LearnMultithreadedRendering/LearnMultithreadedRendering/x64/Debug/vertex.cso");
            auto pixel_shader = this->CreateShader("D:/DiskD/Study/Multithread/LearnMultithreadedRendering/LearnMultithreadedRendering/x64/Debug/pixel.cso");

            // 頂点入力レイアウト
            D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
            {
              // 頂点データ
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            };

            // デフォルト設定のラスタライザーステートの設定
            auto rasterizer_desc = CreateDefaultRasterizerDesc();

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
            D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = {};
            pipeline_state_desc.InputLayout = { inputElementDescs, _countof(inputElementDescs) }; // 入力レイアウトの構造
            pipeline_state_desc.RasterizerState = rasterizer_desc;                                // ラスタライザの状態
            pipeline_state_desc.VS = vertex_shader->Get();                                        // 頂点シェーダーの構造
            pipeline_state_desc.PS = pixel_shader->Get();                                         // ピクセルシェーダーの構造
            pipeline_state_desc.BlendState = blendDesc;                                           // ブレンド状態の構造
            pipeline_state_desc.DepthStencilState = depthStencilDesc;                             // 深度ステンシル状態の構造
            pipeline_state_desc.SampleMask = UINT_MAX;                                            // ブレンドの状態のためのサンプルのマスク
            pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // 入力プリミティブ(三角形)
            pipeline_state_desc.NumRenderTargets = 1;                                             // レンダーターゲットのフォーマット数
            pipeline_state_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                       // レンダーターゲットのフォーマット
            pipeline_state_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                // 深度ステンシルのフォーマット
            pipeline_state_desc.SampleDesc = sampleDesc;                                          // サンプリング状態の構造

                                                                                                  // ルートシグネチャをパイプラインステートに設定する
            root_signature_->SetGraphicsPipelineStateDesc(&pipeline_state_desc);

            // グラフィックスパイプラインステートの生成
            pipeline_state_ = IGraphicsPipelineState::Create(device_.get(), pipeline_state_desc);
          }
        }

        /**
         *  @brief  コマンドキューを作成する
         *  @param  command_queue_desc:コマンドキューの設定
         *  @return コマンドキューへのシェアードポインタ
         */
        std::shared_ptr<ICommandQueue> Device::CreateCommandQueue(const D3D12_COMMAND_QUEUE_DESC& command_queue_desc)
        {
          // コマンドキューの作成
          // コマンドキューはGPUへ描画命令が出されたコマンドリストを
          // 順次実行する
          // つまり特定のアダプター(GPU)に関連付けられている
          return ICommandQueue::Create(device_.get(), command_queue_desc);
        }

        /**
         *  @brief  スワップチェーンを作成する
         *  @param  factory:DXGIのファクトリ
         *  @param  command_queue:コマンドキュー
         *  @param  handle:ウィンドウハンドル
         *  @param  swap_chain_desc:スワップチェーンの設定
         *  @return スワップチェーンへのシェアードポインタ
         */
        std::shared_ptr<ISwapChain> Device::CreateSwapChain(IDXGIFactory4* const factory, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc)
        {
          // TODO:const_castを削除する
          return ISwapChain::Create(factory, const_cast<ID3D12CommandQueue*>(&(command_queue_->Get())), handle, swap_chain_desc);
        }

        /**
         *  @brief  GPUの処理待ちを行う
         */
        void WaitForGpu()
        {
          // TODO:const_castを削除する
          fence->Wait(const_cast<ID3D12CommandQueue*>(&(command_queue_->Get())));
        }

      private:
        std::unique_ptr<ID3D12Device, std::function<void(ID3D12Device*)>> device_;          ///< デバイス
        std::shared_ptr<ISwapChain> swap_chain_;        ///< スワップチェーン
        std::shared_ptr<ICommandQueue> command_queue_;  ///< コマンドキュー


        // TODO:これより下は別クラスに移動する
        std::shared_ptr<IRootSignature> root_signature_;                  ///< ルートシグネチャ
        std::shared_ptr<IGraphicsPipelineState> pipeline_state_;          ///< パイプラインステート
        std::vector<std::shared_ptr<IDescriptorHeap>> descriptor_heaps_;  ///< ディスクリプターヒープ配列
        std::unique_ptr<Fence> fence;                                     ///< フェンス移動
        std::unique_ptr<DepthStencilView> depthStencilView;               ///< 深度ステンシルビュー
        std::vector<std::unique_ptr<TextureView>> texBuffer;              ///<テクスチャバッファ
        std::vector<std::unique_ptr<ID3D12Resource>> render_target_list_; ///< レンダーターゲットのリスト
      };
    };

    /**
     *  @brief  デバイスを生成する
     *  @param  handle:ウィンドウハンドル
     *  @param  width:横幅
     *  @param  height:縦幅
     *  @return デバイスへのシェアードポインタ
     */
    std::shared_ptr<IDevice> IDevice::Create(const HWND handle, const std::uint32_t width, const std::uint32_t height)
    {
      auto device = std::make_shared<Device>();

      device->Create(handle, width, height);

      return device;
    }
  };
};