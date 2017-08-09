/**
 *  @file     descriptor_heap.cc
 *  @brief    ディスクリプターヒープに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/08/09
 *  @version  1.0
 */

 // include
#include "descriptor_heap.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コンストラクタ
     */
    DescriptorHeap::DescriptorHeap() : heap(nullptr), incrementSize(0), createdCount(0), availableCount(0)
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
      createdCount = 0;
      availableCount = desc.NumDescriptors;
    }
    
    /**
     *  @brief  リソースを開放する
     */
    void DescriptorHeap::Release()
    {
      if (heap)
      {
        heap.release()->Release();
      }
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
    D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::CreateDescriptor()
    {
      if (availableCount <= createdCount)
      {
        throw "生成可能なディスクリプターヒープ数を超えています";
      }

      auto handle = heap->GetCPUDescriptorHandleForHeapStart();
      handle.ptr += incrementSize * createdCount;
      ++createdCount;
      return handle;
    }
    
    /**
     *  @brief  生成したディスクリプター数を取得する
     *  @return 生成したディスクリプター数
     */
    unsigned short DescriptorHeap::GetCreatedCount() const
    {
      return createdCount;
    }
    
    /**
     *  @brief  生成可能なディスクリプター数を取得する
     *  @return 生成可能なディスクリプター数
     */
    unsigned short DescriptorHeap::GetAvailableCount() const
    {
      return availableCount - createdCount;
    }
  };
};