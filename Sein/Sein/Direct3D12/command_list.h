/**
 *  @file     command_list.h
 *  @brief    コマンドリストに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2017/08/21
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
       *  @brief  デストラクタ
       */
      virtual ~ICommandList() = default;

      /**
       *  @brief  コマンドリストを生成する
       *  @param  device:Direct3D12のデバイス
       *  @param  type:コマンドリストのタイプ
       */
      virtual void Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type) = 0;

      /**
       *  @brief  リソースを開放する
       */
      virtual void Release() = 0;

      /**
       *  @brief  記録を開始する
       */
      virtual void Begin() = 0;

      /**
       *  @brief  記録を終了する
       */
      virtual void End() = 0;
    };
  };
};