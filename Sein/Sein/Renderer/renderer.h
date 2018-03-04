/**
 *  @file     renderer.h
 *  @brief    レンダラーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/05
 *  @version  1.0
 */

#pragma once

// include
#include <memory>

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  レンダラー用インターフェイス
     */
    class IRenderer
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IRenderer() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IRenderer() = default;

      /**
       *  @brief  レンダラーを作成する
       *  @return レンダラーインターフェイスへのシェアードポインタ
       */
      static std::shared_ptr<IRenderer> Create();

      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IRenderer(const IRenderer& other) = delete;

      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IRenderer& operator = (const IRenderer& other) = delete;
    };
  };
};