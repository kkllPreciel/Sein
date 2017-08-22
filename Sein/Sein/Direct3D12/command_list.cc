/** *  @file     command_list.cc
 *  @brief    コマンドリストに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2017/08/22
 *  @version  1.0
 */

 // include
#include "command_list.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  コンストラクタ
     */
    CommandList::CommandList() : commandAllocator(nullptr, [](IUnknown* p) {p->Release();}), commandList(nullptr, [](IUnknown* p) {p->Release(); })
    {

    }
    
    /**
     *  @brief  デストラクタ
     */
    CommandList::~CommandList()
    {
      Release();
    }
    
    /**
     *  @brief  コマンドリストを生成する
     *  @param  device:Direct3D12のデバイス
     *  @param  type:コマンドリストのタイプ
     */
    void CommandList::Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type)
    {
      Release();

      ID3D12CommandAllocator* allocator;
      if (FAILED(device->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator))))
      {
        throw "コマンドアロケーターの生成に失敗しました。";
      }
      commandAllocator.reset(allocator);

      ID3D12GraphicsCommandList* list;
      if (FAILED(device->CreateCommandList(
        0,                            // マルチアダプター(マルチGPU)の場合に使用するアダプター(GPU)の識別子(単一なので0)
        type,                         // コマンドリストの種別(レンダリング関連のコマンドリスト)
        allocator,                    // このコマンドリストで使用するコマンドアロケータ
        nullptr,                      // コマンドリストの初期パイプライン状態(ダミーの初期パイプラインを指定)
        IID_PPV_ARGS(&list))))
      {
        throw "コマンドリストの生成に失敗しました。";
      }
      commandList.reset(list);

      End();
    }
    
    /**
     *  @brief  リソースを開放する
     */
    void CommandList::Release()
    {
      commandList.reset(nullptr);
      commandAllocator.reset(nullptr);
    }
    
    /**
     *  @brief  記録を開始する
     */
    void CommandList::Begin()
    {
      if (FAILED(commandAllocator->Reset()))
      {
        throw "コマンドアロケーターのリセットに失敗しました。";
      }

      if (FAILED(commandList->Reset(commandAllocator.get(), nullptr)))
      {
        throw "コマンドリストのリセットに失敗しました。";
      }
    }
    
    /**
     *  @brief  記録を終了する
     */
    void CommandList::End()
    {
      commandList->Close();
    }

    /**
     *  @brief  コマンドリストを取得する
     *  @return コマンドリストの参照
     */
    ID3D12GraphicsCommandList& CommandList::Get() const
    {
      return *commandList;
    }
  };
};