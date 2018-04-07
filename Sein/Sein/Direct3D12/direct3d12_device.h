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
#include "swap_chain.h"
#include "root_signature.h"
#include "command_queue.h"
#include "graphics_pipeline_state.h"
#include "command_list.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"

namespace Sein
{
  namespace Direct3D12
  {
    class DepthStencilView;
    class Fence;
    class ConstantBuffer;
    class ShaderResourceBuffer;
    class DescriptorHeap;
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
      void BeginScene(ICommandList* const command_list, std::uint32_t buffer_index);

      /**
       *  @brief  シーンを終了する
       */
      void EndScene(ICommandList* const command_list, std::uint32_t buffer_index);

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
       *  @brief  コマンドリストを作成する
       *  @param  command_list_type:コマンドリストのタイプ
       *  @return コマンドリストのシェアードポインタ
       */
      std::shared_ptr<ICommandList> CreateCommandList(const D3D12_COMMAND_LIST_TYPE& command_list_type);

      /**
       *  @brief  頂点バッファを作成する
       *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
       *  @return 頂点バッファへのユニークポインタ
       */
      std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes);

      /**
       *  @brief  頂点インデックスバッファを作成する
       *  @param  size_in_bytes:頂点インデックスバッファのサイズ(頂点インデックスサイズ * 頂点インデックス数)
       *  @return 頂点インデックスバッファへのユニークポインタ
       */
      std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes);

      /**
       *  @brief  シェーダーを作成する
       *  @param  shader_file_path:シェーダーファイルのパス
       *  @return シェーダーへのシェアードポインタ
       */
      std::shared_ptr<IShader> CreateShader(const std::string& shader_file_path);

      /**
       *  @brief  デバイスを取得する
       *  @return デバイスへの参照
       */
      ID3D12Device& GetDevice() const;

      /**
       *  @brief  次にバックバッファになるバッファの番号を取得する
       *  @return 次にバックバッファになるバッファの番号
       */
      const std::uint32_t GetNextBackBufferIndex() const;

    private:
      /**
       *  @brief  スワップチェーンを作成する
       *  @param  factory:DXGIのファクトリ
       *  @param  command_queue:コマンドキュー
       *  @param  handle:ウィンドウハンドル
       *  @param  swap_chain_desc:スワップチェーンの設定
       *  @return スワップチェーンへのシェアードポインタ
       */
      std::shared_ptr<ISwapChain> CreateSwapChain(IDXGIFactory4* const factory, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc);

      /**
       *  @brief  コマンドキューを作成する
       *  @param  command_queue_desc:コマンドキューの設定
       *  @return コマンドキューへのシェアードポインタ
       */
      std::shared_ptr<ICommandQueue> CreateCommandQueue(const D3D12_COMMAND_QUEUE_DESC& command_queue_desc);

    private:
      std::unique_ptr<ID3D12Device, void(*)(ID3D12Device*)> device_;  ///< デバイス
      std::shared_ptr<ICommandQueue> command_queue_;                  ///< コマンドキュー
      std::shared_ptr<ISwapChain> swap_chain_;                        ///< スワップチェーン

      // TODO:外部へ移動する
      std::shared_ptr<IRootSignature> root_signature_;                ///< ルートシグネチャ
      std::shared_ptr<IGraphicsPipelineState> pipeline_state_;        ///< パイプラインステート

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

    public:
      /**
       *  @brief  描画する
       *  @param  command_list:コマンドリスト
       *  @param  indexCount:頂点インデックス数
       *  @param  instanceCount:インスタンス数
       */
      void Render(ICommandList* const command_list, const unsigned int indexCount, const unsigned int instanceCount);

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

#pragma region CommandQueue
      /**
       *  @brief  コマンドリストを実行する
       *  @param  command_list:コマンドリスト
       */
      void ExecuteCommandLists(ICommandList* const command_list);
#pragma endregion
    };
  };
};