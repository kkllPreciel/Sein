/**
 *  @file     command_queue.cc
 *  @brief    コマンドキューに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

 // include
#include "direct3d12_command_queue.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  コマンドキュー用クラス
       */
      class CommandQueue final : public ICommandQueue
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        CommandQueue() : command_queue_(nullptr, [](ID3D12CommandQueue* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~CommandQueue() override
        {
          Release();
        }

        /**
         *  @brief  コマンドキューを設定する
         *  @param  command_queue:コマンドキュー
         */
        void SetCommandQueue(ID3D12CommandQueue* command_queue)
        {
          command_queue_.reset(command_queue);
        }

        /**
         *  @brief  コマンドリストを実行する
         *  @param  command_list_count:コマンドリストの数
         *  @param  command_list:の配列
         */
        void ExecuteCommandLists(std::uint32_t command_list_count, ID3D12CommandList* const* command_list) override
        {
          command_queue_->ExecuteCommandLists(command_list_count, command_list);
        }

        /**
         *  @brief  コマンドキューを取得する
         *  @return コマンドキュー
         */
        const ID3D12CommandQueue& Get() const override
        {
          return *command_queue_;
        }

        /**
         *  @brief  コマンドキューを開放する
         */
        void Release() noexcept override
        {
          command_queue_.reset();
        }

      private:
        std::unique_ptr<ID3D12CommandQueue, void(*)(ID3D12CommandQueue*)> command_queue_; ///< コマンドキュー
      };
    };

    /**
     *  @brief  コマンドキューを作成する
     *  @param  device:Direct3D12のデバイス
     *  @param  command_queue_desc:コマンドキューの設定
     *  @return コマンドキューへのシェアードポインタ
     */
    std::shared_ptr<ICommandQueue> ICommandQueue::Create(ID3D12Device* const device, const D3D12_COMMAND_QUEUE_DESC& command_queue_desc)
    {
      ID3D12CommandQueue* queue;
      if (FAILED(device->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&queue))))
      {
        throw std::exception("コマンドキューの生成に失敗しました。");
      }

      auto instance = std::make_unique<CommandQueue>();
      instance->SetCommandQueue(queue);
      return instance;
    }
  };
};