/**
 *  @file     command_list.cc
 *  @brief    コマンドリストに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/26
 *  @version  1.0
 */

 // include
#include <functional>
#include "command_list.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  コマンドリスト用クラス
       */
      class CommandList final : public ICommandList
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        CommandList() : command_allocator_(nullptr, [](ID3D12CommandAllocator* p) { p->Release(); }), command_list_(nullptr, [](ID3D12GraphicsCommandList* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~CommandList() override
        {
          Release();
        }

        /**
         *  @brief  コマンドアロケータを設定する
         *  @param  command_allocator:コマンドアロケータ
         */
        void SetCommandAllocator(ID3D12CommandAllocator* command_allocator)
        {
          command_allocator_.reset(command_allocator);
        }

        /**
         *  @brief  コマンドリストを設定する
         *  @param  command_list:コマンドリスト
         */
        void SetCommandList(ID3D12GraphicsCommandList* command_list)
        {
          command_list_.reset(command_list);
        }

        /**
         *  @brief  記録を開始する
         */
        void Begin() override
        {
          if (FAILED(command_allocator_->Reset()))
          {
            throw std::exception("コマンドアロケーターのリセットに失敗しました。");
          }

          if (FAILED(command_list_->Reset(command_allocator_.get(), nullptr)))
          {
            throw std::exception("コマンドリストのリセットに失敗しました。");
          }
        }

        /**
         *  @brief  リソースの状態遷移に対してバリアを設定する
         *  @param  resource:バリアを設定するリソース
         *  @param  before:状態遷移前のリソースの状態
         *  @param  after:状態遷移後のリソースの状態
         */
        void ResourceBarrier(ID3D12Resource* const resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after) override
        {
          // バリアを設定してリソースの状態遷移を行う
          D3D12_RESOURCE_BARRIER barrier;
          barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;                    // バリアはリソースの状態遷移に対して設置
          barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;                         // TODO:調べる
          barrier.Transition.pResource = resource;                                  // バリアを設定するリソース
          barrier.Transition.StateBefore = before;                                  // 状態遷移前の状態
          barrier.Transition.StateAfter = after;                                    // 状態遷移後の状態
          barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

          command_list_->ResourceBarrier(1, &barrier);
        }

        /**
         *  @brief  頂点バッファを設定する
         *  @param  start_slot:開始スロット番号
         *  @param  vertex_buffer_count:頂点バッファの数
         *  @param  vertex_buffers:頂点バッファの配列
         */
        void SetVertexBuffers(const std::uint32_t start_slot, std::uint32_t vertex_buffer_count, const D3D12_VERTEX_BUFFER_VIEW* vertex_buffers) override
        {
          // 入力アセンブラーステージに頂点バッファをバインドする
          command_list_->IASetVertexBuffers(start_slot, vertex_buffer_count, vertex_buffers);
        }

        /**
         *  @brief  インデックスバッファを設定する
         *  @param  index_buffer:インデックスバッファ
         */
        void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* index_buffer) override
        {
          // 入力アセンブラーステージにインデックスバッファをバインドする
          command_list_->IASetIndexBuffer(index_buffer);
        }

        /**
         *  @brief  プリミティブのトポロジーを設定する
         *  @param  primitive_topology:プリミティブのトポロジー
         */
        void SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY primitive_topology) override
        {
          command_list_->IASetPrimitiveTopology(primitive_topology);
        }
        
        /**
         *  @brief  記録を終了する
         */
        void End() override
        {
          if (FAILED(command_list_->Close()))
          {
            throw std::exception("コマンドリストのクローズに失敗しました。");
          }
        }
        
        /**
         *  @brief  コマンドリストを取得する
         *  @return コマンドリストの参照
         */
        const ID3D12GraphicsCommandList& Get() const override
        {
          return *command_list_;
        }
        
        /**
         *  @brief  リソースを開放する
         */
        void Release() noexcept override
        {
          command_list_.reset();
          command_allocator_.reset();
        }

      private:
        std::unique_ptr<ID3D12CommandAllocator, std::function<void(ID3D12CommandAllocator*)>> command_allocator_;   ///< コマンドアロケータ
        std::unique_ptr<ID3D12GraphicsCommandList, std::function<void(ID3D12GraphicsCommandList*)>> command_list_;  ///< コマンドリスト
      };
    };

    /**
     *  @brief  コマンドリストを作成する
     *  @param  device:Direct3D12のデバイス
     *  @param  command_list_type:コマンドリストのタイプ
     *  @return コマンドリストへのシェアードポインタ
     */
    std::shared_ptr<ICommandList> ICommandList::Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& command_list_type)
    {
      ID3D12CommandAllocator* command_allocator;
      if (FAILED(device->CreateCommandAllocator(command_list_type, IID_PPV_ARGS(&command_allocator))))
      {
        throw std::exception("コマンドアロケーターの生成に失敗しました。");
      }

      ID3D12GraphicsCommandList* command_list;
      if (FAILED(device->CreateCommandList(
        0,                          // マルチアダプター(マルチGPU)の場合に使用するアダプター(GPU)の識別子(単一なので0)
        command_list_type,          // コマンドリストの種別(レンダリング関連のコマンドリスト)
        command_allocator,          // このコマンドリストで使用するコマンドアロケータ
        nullptr,                    // コマンドリストの初期パイプライン状態(ダミーの初期パイプラインを指定)
        IID_PPV_ARGS(&command_list) // 作成したコマンドアロケータの格納先ポインタ
      )))
      {
        command_allocator->Release();
        throw std::exception("コマンドリストの生成に失敗しました。");
      }

      auto instance = std::make_unique<CommandList>();
      instance->SetCommandAllocator(command_allocator);
      instance->SetCommandList(command_list);
      instance->End();
      return instance;
    }
  };
};