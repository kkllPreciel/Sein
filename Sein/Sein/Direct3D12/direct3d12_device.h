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
      ID3D12Device*               device;                                   ///< デバイス
      IDXGISwapChain3*            swapChain;                                ///< スワップチェイン
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

      ID3D12RootSignature*  rootSignature;  ///< ルートシグネチャ

#pragma endregion

      // パイプラインステート関連
      // 後々別クラスへ移動
#pragma region PipelineState
      ID3D12PipelineState*  pipelineState;  ///< パイプラインステート

    public:
      /**
       *  @brief  描画する
       *  @param  vertexBuffer:頂点バッファ
       *  @param  indexBuffer:頂点インデックスバッファ
       *  @param  instanceCount:インスタンス数
       */
      void Render(const VertexBuffer& vertebBuffer, const IndexBuffer& indexBuffer, const unsigned int instanceCount);
#pragma endregion

      // テクスチャ関連
      // 後々別クラスへ移動
#pragma region Texture
      std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> texBuffer;  ///<テクスチャバッファ

      /**
       *  @brief  テクスチャバッファを生成する
       */
      void CreateTextureBuffer();
#pragma endregion
    };
  };
};