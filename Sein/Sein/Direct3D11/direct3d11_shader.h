/**
 *  @file     direct3d11_shader.h
 *  @brief    Direct3D11のシェーダーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/11
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
     *  @brief  Direct3D11のシェーダー用インターフェース
     */
    class IShader
    {
    public:
      /**
       *  @brief  シェーダー種別
       */
      enum class Type : std::uint32_t {
        Vertex,   ///< 頂点シェーダー
        Pixel,    ///< ピクセル(フラグメント)シェーダー
        Geometry, ///< ジオメトリシェーダー
        Hull,     ///< ハルシェーダー
        Domain,   ///< ドメインシェーダー
        Compute,  ///< コンピュートシェーダー
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
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  シェーダーを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  shader_desc:シェーダーの設定
       *  @return シェーダーへのシェアードポインタ
       */
      static std::shared_ptr<IShader> Create(ID3D11Device* const device, const Desc& shader_desc);
    };
  };
};