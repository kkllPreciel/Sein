/**
 *  @file     buffer.h
 *  @brief    Direct3D11のバッファに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/03
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11のバッファ用インターフェイス
     */
    class IBuffer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IBuffer() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IBuffer() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IBuffer(const IBuffer& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IBuffer& operator = (const IBuffer& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  バッファを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  buffer_desc:バッファの設定
       *  @return バッファへのシェアードポインタ
       */
      static std::shared_ptr<IBuffer> Create(ID3D11Device* const device, const D3D11_BUFFER_DESC& buffer_desc);
    };
  };
};