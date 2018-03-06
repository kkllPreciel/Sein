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
       *  @brief  初期化する
       *  @return 初期化成功フラグ
       */
      virtual bool Initialize() = 0;

      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

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