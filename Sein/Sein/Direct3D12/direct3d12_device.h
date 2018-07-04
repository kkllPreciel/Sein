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
#include "direct3d12_command_list.h"
#include "direct3d12_constant_buffer.h"
#include "direct3d12_shader_resource_buffer.h"
#include "direct3d12_vertex_buffer.h"
#include "direct3d12_index_buffer.h"
#include "direct3d12_shader.h"
#include "direct3d12_descriptor_heap.h"
#include "direct3d12_depth_stencil_view.h"

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
       *  @param  descriptor_heap:ディスクリプターヒープ
       *  @param  index_count:頂点インデックス数
       *  @param  instance_count:インスタンス数
       */
      virtual void Render(ICommandList* const command_list, std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t index_count, const std::uint32_t instance_count) = 0;

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
       *  @param  descriptor_heap:定数バッファを作成するディスクリプターヒープ
       *  @param  size_in_bytes:定数バッファのサイズ
       *  @return 定数バッファのユニークポインタ
       */
      virtual std::unique_ptr<IConstantBuffer> CreateConstantBuffer(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t size_in_bytes) = 0;

      /**
       *  @brief  シェーダーリソースバッファを作成する
       *  @param  descriptor_heap:シェーダーリソースバッファを作成するディスクリプターヒープ
       *  @param  num:リソース内の要素数
       *  @param  size:リソース内の1要素のサイズ
       *  @return シェーダーリソースバッファのユニークポインタ
       */
      virtual std::unique_ptr<ShaderResourceBuffer> CreateShaderResourceBuffer(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t num, const std::uint32_t size) = 0;

      /**
       *  @brief  深度ステンシルバッファを作成する
       *  @param  descriptor_heap:深度ステンシルバッファを作成するディスクリプターヒープ
       *  @param  width:深度ステンシルバッファの横幅
       *  @param  height:深度ステンシルバッファの縦幅
       *  @return 深度ステンシルバッファのユニークポインタ
       */
      virtual std::unique_ptr<DepthStencilView> CreateDepthStencilView(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t width, const std::uint32_t height) = 0;

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

      /**
       *  @brief  ディスクリプターヒープを生成する
       *  @param  descriptor_heap_desc:ディスクリプターヒープの設定
       *  @return ディスクリプターヒープのシェアードポインタ
       */
      virtual std::shared_ptr<IDescriptorHeap> CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& descriptor_heap_desc) = 0;

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