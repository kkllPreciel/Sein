/**
 *  @file     shader.h
 *  @brief    レンダラーのシェーダーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/07/04
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief シェーダー用インターフェース
     */
    class IShader
    {
    public:
      /**
       *  @brief  シェーダー種別
       */
      enum class Type : std::uint32_t {
        kVertex,    ///< 頂点シェーダー
        kPixel,     ///< ピクセル(フラグメント)シェーダー
        kGeometry,  ///< ジオメトリシェーダー
        kHull,      ///< ハルシェーダー
        kDomain,    ///< ドメインシェーダー
        kCompute,   ///< コンピュートシェーダー
      };

      /**
       *  @brief  シェーダーの設定
       */
      class Desc
      {
      public:
        const void* shader_bytecode;  ///< シェーダーバイトコード
        std::uint32_t bytecode_size;  ///< シェーダーバイトコードのサイズ
        Type shader_type;             ///< シェーダー種別
      };

      /**
       *  @brief  コンストラクタ
       */
      IShader() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IShader() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IShader(const IShader& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IShader& operator = (const IShader& other) = delete;

      /**
       *  @brief  シェーダーの設定を取得する
       */
      virtual const Desc& GetDesc() = 0;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;
    };
  };
};