/**
 *  @file     direct3d12_device.h
 *  @brief    Direct3D12のデバイスに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/02/06
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <vector>
#include <wrl\client.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include "root_signature.h"

namespace Sein
{
  namespace Direct3D12
  {
    class VertexBuffer;
    class IndexBuffer;
    class DepthStencilView;
    class Fence;
    class ConstantBuffer;
    class ShaderResourceBuffer;
    class DescriptorHeap;
    class CommandList;
    class TextureView;

    /**
     *  @brief  Direct3D12用デバイスクラス(スワップチェインも一緒)
     */
    class Device
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      Device();

      /**
       *  @brief  デストラクタ
       */
      ~Device();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      Device(const Device& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      Device& operator = (const Device& other) = delete;

      /**
       *  @brief  デバイスを生成する
       *  @param  handle:ウィンドウハンドル
       *  @param  width:横幅
       *  @param  height:縦幅
       */
      void Create(HWND handle, unsigned int width, unsigned int height);

      /**
       *  @brief  デバイスを開放する
       */
      void Release();

      /**
       *  @brief  シーンを開始する
       */
      void BeginScene();

      /**
       *  @brief  シーンを終了する
       */
      void EndScene();

      /**
       *  @brief  画面を更新する
       */
      void Present();

      /**
       *  @brief  定数バッファを作成する
       *  @param  size:定数バッファのサイズ
       *  @return 定数バッファのユニークポインタ
       */
      std::unique_ptr<ConstantBuffer> CreateConstantBuffer(const unsigned int size);

      /**
       *  @brief  シェーダーリソースバッファを作成する
       *  @param  num:リソース内の要素数
       *  @param  size:リソース内の1要素のサイズ
       *  @return シェーダーリソースバッファのユニークポインタ
       */
      std::unique_ptr<ShaderResourceBuffer> CreateShaderResourceBuffer(const unsigned int num, const unsigned int size);

      /**
       *  @brief  デバイスを取得する
       *  @return デバイスへの参照
       */
      ID3D12Device& GetDevice() const;

    private:
      std::unique_ptr<ID3D12Device, void(*)(IUnknown*)> device;             ///< デバイス
      std::unique_ptr<IDXGISwapChain3, void(*)(IUnknown*)> swapChain;       ///< スワップチェイン
      std::unique_ptr<ID3D12CommandQueue, void(*)(IUnknown*)> commandQueue; ///< コマンドキュー
      std::unique_ptr<CommandList> commandList;                             ///< コマンドリスト
      std::unique_ptr<DescriptorHeap[]> descriptorHeaps;                    ///< ディスクリプターヒープ配列
      std::unique_ptr<Fence> fence;                                         ///< フェンス

      // 後々ダブルバッファクラスへ移動
      static const unsigned int FrameCount = 2;               ///< フレーム数(ダブルバッファ)
      ID3D12Resource*           renderTargetList[FrameCount]; ///< レンダーターゲットのリスト
      unsigned int              bufferIndex;                  ///< 現在のバッファ番号

      // 深度ステンシル後々外部へ移動
      std::unique_ptr<DepthStencilView> depthStencilView; ///< 深度ステンシルビュー

      /**
       *  @brief  描画終了待ちを行う
       */
      void WaitForGpu();

      /**
       *  @brief  アセットを読み込む
       *  @param  width:ウィンドウ横幅
       *  @param  height:ウィンドウ縦幅
       */
      void LoadAssets(unsigned int width, unsigned int height);

      // ルートシグネチャ関連
      // 後々別クラスへ移動
#pragma region RootSignature
      std::shared_ptr<IRootSignature> root_signature_;  ///< ルートシグネチャ
#pragma endregion

      // パイプラインステート関連
      // 後々別クラスへ移動
#pragma region PipelineState
      ID3D12PipelineState*  pipelineState;  ///< パイプラインステート

    public:
      /**
       *  @brief  描画する
       *  @param  indexCount:頂点インデックス数
       *  @param  instanceCount:インスタンス数
       */
      void Render(const unsigned int indexCount, const unsigned int instanceCount);

      /**
       *  @brief  頂点バッファを設定する
       *  @param  start_slot:開始スロット番号
       *  @param  vertex_buffer_count:頂点バッファの数
       *  @param  vertex_buffers:頂点バッファの配列
       */
      void SetVertexBuffers(const UINT start_slot, const UINT vertex_buffer_count, const D3D12_VERTEX_BUFFER_VIEW* vertex_buffers);

      /**
       *  @brief  インデックスバッファを設定する
       *  @param  index_buffer:インデックスバッファ
       */
      void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* index_buffer);

      /**
       *  @brief  プリミティブのタイプを設定する
       *  @param  topology:プリミティブのタイプ
       */
      void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);
#pragma endregion

      // テクスチャ関連
      // 後々別クラスへ移動
#pragma region Texture
      std::vector<std::unique_ptr<TextureView>> texBuffer;  ///<テクスチャバッファ

      /**
       *  @brief  ファイルからテクスチャバッファを生成する
       *  @param  file_path:テクスチャファイルパス
       */
      void CreateTextureBufferFromFile(const std::wstring& file_path);
#pragma endregion
    };
  };
};