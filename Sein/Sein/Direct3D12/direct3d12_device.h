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
#include "command_list.h"
#include "constant_buffer.h"
#include "shader_resource_buffer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  Direct3D12デバイス用インターフェイス
     */
    class IDevice
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IDevice() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IDevice() = default;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IDevice(const IDevice& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IDevice& operator = (const IDevice& other) = delete;

      /**
       *  @brief  デバイスを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  シーンを開始する
       */
      virtual void BeginScene(ICommandList* const command_list, std::uint32_t buffer_index) = 0;

      /**
       *  @brief  描画する
       *  @param  command_list:コマンドリスト
       *  @param  indexCount:頂点インデックス数
       *  @param  instanceCount:インスタンス数
       */
      virtual void Render(ICommandList* const command_list, const unsigned int indexCount, const unsigned int instanceCount) = 0;

      /**
       *  @brief  シーンを終了する
       */
      virtual void EndScene(ICommandList* const command_list, std::uint32_t buffer_index) = 0;

      /**
       *  @brief  画面を更新する
       */
      virtual void Present() = 0;

      /**
       *  @brief  定数バッファを作成する
       *  @param  size_in_bytes:定数バッファのサイズ
       *  @return 定数バッファのユニークポインタ
       */
      virtual std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const std::uint32_t size_in_bytes) = 0;

      /**
       *  @brief  シェーダーリソースバッファを作成する
       *  @param  num:リソース内の要素数
       *  @param  size:リソース内の1要素のサイズ
       *  @return シェーダーリソースバッファのユニークポインタ
       */
      virtual std::unique_ptr<ShaderResourceBuffer> CreateShaderResourceBuffer(const unsigned int num, const unsigned int size) = 0;

      /**
       *  @brief  コマンドリストを作成する
       *  @param  command_list_type:コマンドリストのタイプ
       *  @return コマンドリストのシェアードポインタ
       */
      virtual std::shared_ptr<ICommandList> CreateCommandList(const D3D12_COMMAND_LIST_TYPE& command_list_type) = 0;

      /**
       *  @brief  頂点バッファを作成する
       *  @param  size_in_bytes:頂点バッファのサイズ(頂点サイズ * 頂点数)
       *  @return 頂点バッファへのユニークポインタ
       */
      virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) = 0;

      /**
       *  @brief  頂点インデックスバッファを作成する
       *  @param  size_in_bytes:頂点インデックスバッファのサイズ(頂点インデックスサイズ * 頂点インデックス数)
       *  @return 頂点インデックスバッファへのユニークポインタ
       */
      virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) = 0;

      /**
       *  @brief  シェーダーを作成する
       *  @param  shader_file_path:シェーダーファイルのパス
       *  @return シェーダーへのシェアードポインタ
       */
      virtual std::shared_ptr<IShader> CreateShader(const std::string& shader_file_path) = 0;

      /**
       *  @brief  次にバックバッファになるバッファの番号を取得する
       *  @return 次にバックバッファになるバッファの番号
       */
      virtual const std::uint32_t GetNextBackBufferIndex() const noexcept = 0;

      /**
       *  @brief  ファイルからテクスチャバッファを生成する
       *  @param  file_path:テクスチャファイルパス
       */
      virtual void CreateTextureBufferFromFile(const std::wstring& file_path) = 0;

      /**
       *  @brief  コマンドリストを実行する
       *  @param  command_list:コマンドリスト
       */
      virtual void ExecuteCommandLists(ICommandList* const command_list) = 0;


      // TODO:ディスクリプターヒープを作成するインターフェイス
      // TODO:レンダーターゲットを作成するインターフェイス



      /**
       *  @brief  デバイスを生成する
       *  @param  handle:ウィンドウハンドル
       *  @param  width:横幅
       *  @param  height:縦幅
       *  @return デバイスへのシェアードポインタ
       */
      static std::shared_ptr<IDevice> Create(const HWND handle, const std::uint32_t width, const std::uint32_t height);
    };
  };
};