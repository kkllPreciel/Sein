/**
 *  @file     descriptor_heap.cc
 *  @brief    ディスクリプターヒープに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/08/09
 *  @version  1.0
 */

 // include
#include "direct3d12_descriptor_heap.h"
#include <vector>
#include <functional>

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  ディスクリプターヒープ用クラス
       */
      class DescriptorHeap final : public IDescriptorHeap
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        DescriptorHeap() : descriptor_heap_(nullptr, [](IUnknown* p) {p->Release(); }), descriptors_(), descriptor_increment_size_(0), available_count_(0), created_count_(0)
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~DescriptorHeap() override
        {
          Release();
        }

        /**
         *  @brief  生成する
         *  @param  device:Direct3D12のデバイス
         *  @param  desc:ディスクリプターヒープの設定情報
         */
        void Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
        {
          Release();

          ID3D12DescriptorHeap* descriptor_heap = nullptr;
          if (FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptor_heap))))
          {
            throw std::exception("ディスクリプターヒープの生成に失敗しました。");
          }

          descriptor_heap_.reset(descriptor_heap);
          descriptor_increment_size_ = device->GetDescriptorHandleIncrementSize(desc.Type);
          descriptors_.resize(desc.NumDescriptors);
          available_count_ = desc.NumDescriptors;
          created_count_ = 0;
        }

        /**
         *  @brief  リソースを開放する
         */
        void Release() noexcept override
        {
          descriptors_.clear();
          descriptor_heap_.reset(nullptr);
        }
        
        /**
         *  @brief  ディスクリプターを生成する
         *  @return ディスクリプターハンドル
         */
        const IDescriptor& CreateDescriptor() override
        {
          if (GetAvailableCount() <= 0)
          {
            throw std::exception("生成可能なディスクリプター数を超えています");
          }

          auto handle_for_cpu = descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
          auto handle_for_gpu = descriptor_heap_->GetGPUDescriptorHandleForHeapStart();
          handle_for_cpu.ptr += descriptor_increment_size_ * created_count_;
          handle_for_gpu.ptr += descriptor_increment_size_ * created_count_;

          descriptors_[created_count_] = IDescriptor::Create(handle_for_cpu, handle_for_gpu);
          return *(descriptors_[created_count_++]);
        }
        
        /**
         *  @brief  ディスクリプターを取得する
         *  @param  index:ディスクリプター番号
         *  @return ディスクリプターハンドル
         */
        const IDescriptor& GetDescriptor(const std::uint32_t index) override
        {
          if (created_count_ <= index)
          {
            throw std::exception("指定されたインデックスが生成済みのディスクリプター数を超えています");
          }

          return *(descriptors_[index]);
        }
        
        /**
         *  @brief  生成したディスクリプター数を取得する
         *  @return 生成したディスクリプター数
         */
        std::uint32_t GetCreatedCount() const override
        {
          return created_count_;
        }
        
        /**
         *  @brief  生成可能なディスクリプター数を取得する
         *  @return 生成可能なディスクリプター数
         */
        std::uint32_t GetAvailableCount() const override
        {
          return available_count_ - created_count_;
        }

        /**
         *  @brief  コマンドリストにディスクリプターヒープを設定する
         *  @param  command_list:コマンドリスト
         */
        void SetDescriptorHeaps(ICommandList* const command_list) const override
        {
          auto descriptor_heap = descriptor_heap_.get();
          command_list->SetDescriptorHeaps(1, &(descriptor_heap));
        }

      private:
        std::unique_ptr<ID3D12DescriptorHeap, std::function<void(ID3D12DescriptorHeap*)>> descriptor_heap_; ///< ディスクリプターヒープ
        std::vector<std::shared_ptr<IDescriptor>> descriptors_;                                             ///< ディスクリプターの配列
        std::uint32_t descriptor_increment_size_;                                                           ///< ディスクリプターハンドルのインクリメントサイズ
        std::uint32_t available_count_;                                                                     ///< 生成可能なディスクリプター数
        std::uint32_t created_count_;                                                                       ///< 生成したディスクリプター数
      };
    };

    /**
     *  @brief  ディスクリプターヒープを生成する
     *  @param  device:Direct3D12のデバイス
     *  @param  desc:ディスクリプターヒープの設定情報
     *  @return ディスクリプターヒープへのシェアードポインタ
     */
    std::shared_ptr<IDescriptorHeap> IDescriptorHeap::Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
    {
      auto descriptor_heap = std::make_shared<DescriptorHeap>();

      descriptor_heap->Create(device, desc);

      return descriptor_heap;
    }
  };
};