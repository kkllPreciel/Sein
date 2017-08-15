/**
 *  @file     descriptor_heap.cc
 *  @brief    ディスクリプターヒープに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/08/09
 *  @version  1.0
 */

 // include
#include "descriptor_heap.h"
#include "descriptor.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コンストラクタ
     */
    DescriptorHeap::DescriptorHeap() : heap(nullptr, [](IUnknown* p) {p->Release();}), descriptors(nullptr), incrementSize(0), availableCount(0), createdCount(0)
    {

    }

    /**
     *  @brief  デストラクタ
     */
    DescriptorHeap::~DescriptorHeap()
    {
      Release();
    }
    
    /**
     *  @brief  ディスクリプターヒープを生成する
     *  @param  device:Direct3D12のデバイス
     *  @param  desc:ディスクリプターヒープの設定情報
     */
    void DescriptorHeap::Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
    {
      Release();

      ID3D12DescriptorHeap* descriptor_heap = nullptr;
      auto hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptor_heap));
      if (FAILED(hr))
      {
        throw "ディスクリプターヒープの生成に失敗しました。";
      }
      heap.reset(descriptor_heap);
      incrementSize = device->GetDescriptorHandleIncrementSize(desc.Type);
      descriptors = std::make_unique<Descriptor[]>(desc.NumDescriptors);
      availableCount = desc.NumDescriptors;
      createdCount = 0;
    }
    
    /**
     *  @brief  リソースを開放する
     */
    void DescriptorHeap::Release()
    {
      heap.reset(nullptr);
      descriptors.reset(nullptr);
    }
    
    /**
     *  @brief  ディスクリプターヒープを取得する
     *  @return ディスクリプターヒープへのポインタ
     */
    ID3D12DescriptorHeap* DescriptorHeap::Get() const
    {
      return heap.get();
    }

    /**
     *  @brief  ディスクリプターを生成する
     *  @return ディスクリプターハンドル
     */
    const IDescriptor& DescriptorHeap::CreateDescriptor()
    {
      if (availableCount <= createdCount)
      {
        throw "生成可能なディスクリプター数を超えています";
      }

      auto handleForCPU = heap->GetCPUDescriptorHandleForHeapStart();
      auto handleForGPU = heap->GetGPUDescriptorHandleForHeapStart();
      handleForCPU.ptr += incrementSize * createdCount;
      handleForGPU.ptr += incrementSize * createdCount;
      descriptors[createdCount].Create(handleForCPU, handleForGPU);
      return descriptors[createdCount++];
    }
    
    /**
     *  @brief  ディスクリプターを取得する
     *  @param  index:ディスクリプター番号
     *  @return ディスクリプターハンドル
     */
    const IDescriptor& DescriptorHeap::GetDescriptor(const unsigned int index)
    {
      if (createdCount <= index)
      {
        throw "指定されたインデックスが生成済みのディスクリプター数を超えています";
      }

      return descriptors[index];
    }
    
    /**
     *  @brief  生成したディスクリプター数を取得する
     *  @return 生成したディスクリプター数
     */
    unsigned short DescriptorHeap::GetCreatedCount() const
    {
      return static_cast<unsigned short>(createdCount);
    }
    
    /**
     *  @brief  生成可能なディスクリプター数を取得する
     *  @return 生成可能なディスクリプター数
     */
    unsigned short DescriptorHeap::GetAvailableCount() const
    {
      return availableCount - static_cast<unsigned int>(createdCount);
    }
  };
};