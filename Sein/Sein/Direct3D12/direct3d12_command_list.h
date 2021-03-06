/**
 *  @file     command_list.h
 *  @brief    コマンドリストに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/26
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コマンドリスト用interface
     */
    class ICommandList
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      ICommandList() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~ICommandList() = default;

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      ICommandList(const ICommandList& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      ICommandList& operator = (const ICommandList& other) = delete;

      /**
       *  @brief  記録を開始する
       */
      virtual void Begin() = 0;

      /**
       *  @brief  リソースの状態遷移に対してバリアを設定する
       *  @param  resource:バリアを設定するリソース
       *  @param  before:状態遷移前のリソースの状態
       *  @param  after:状態遷移後のリソースの状態
       */
      virtual void ResourceBarrier(ID3D12Resource* const resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after) = 0;

      /**
       *  @brief  頂点バッファを設定する
       *  @param  start_slot:開始スロット番号
       *  @param  vertex_buffer_count:頂点バッファの数
       *  @param  vertex_buffers:頂点バッファの配列
       */
      virtual void SetVertexBuffers(const std::uint32_t start_slot, std::uint32_t vertex_buffer_count, const D3D12_VERTEX_BUFFER_VIEW* vertex_buffers) = 0;

      /**
       *  @brief  インデックスバッファを設定する
       *  @param  index_buffer:インデックスバッファ
       */
      virtual void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* index_buffer) = 0;

      /**
       *  @brief  プリミティブのトポロジーを設定する
       *  @param  primitive_topology:プリミティブのトポロジー
       */
      virtual void SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY primitive_topology) = 0;

      /**
       *  @brief  ビューポートを設定する
       *  @param  viewport_count:ビューポートの個数
       *  @param  viewports:ビューポート配列の先頭ポインタ
       */
      virtual void SetViewports(const std::uint32_t viewport_count, const D3D12_VIEWPORT* const viewports) = 0;

      /**
       *  @brief  シザー矩形を設定する
       *  @param  scissor_rect_count:シザー矩形の個数
       *  @param  scissor_rects:シザー矩形配列の先頭ポインタ
       */
      virtual void SetScissorRects(const std::uint32_t scissor_rect_count, const D3D12_RECT* const scissor_rects) = 0;

      /**
       *  @brief  ディスクリプターヒープを設定する
       *  @param  descriptor_heap_count:ディスクリプターヒープの個数
       *  @param  descriptor_heaps:ディスクリプターヒープ配列の先頭ポインタ
       */
      virtual void SetDescriptorHeaps(const std::uint32_t descriptor_heap_count, ID3D12DescriptorHeap* const* descriptor_heaps) = 0;

      /**
       *  @brief  ディスクリプターテーブルを設定する
       *  @param  root_parameter_index:ルートパラメータの番号
       *  @param  descriptor_handle:ディスクリプターのハンドル
       */
      virtual void SetGraphicsRootDescriptorTable(const std::uint32_t root_parameter_index, D3D12_GPU_DESCRIPTOR_HANDLE descriptor_handle) = 0;

      /**
       *  @brief  記録を終了する
       */
      virtual void End() = 0;

      /**
       *  @brief  コマンドリストを取得する
       *  @return コマンドリストの参照
       */
      virtual const ID3D12GraphicsCommandList& Get() const = 0;

      /**
       *  @brief  リソースを開放する
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  コマンドリストを作成する
       *  @param  device:Direct3D12のデバイス
       *  @param  command_list_type:コマンドリストのタイプ
       *  @return コマンドリストへのシェアードポインタ
       */
      static std::shared_ptr<ICommandList> Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& command_list_type);
    };
  };
};