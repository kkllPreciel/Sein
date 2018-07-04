/**
 *  @file     shader.h
 *  @brief    シェーダーに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <string>
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  シェーダー用インターフェイス
     */
    class IShader
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IShader() = default;

      /**
       *  @brief  デストラクタ
       */
      virtual ~IShader() = default;

      /**
       *  @brief  シェーダーを取得する
       *  @return シェーダー
       */
      virtual const D3D12_SHADER_BYTECODE& Get() const = 0;

      /**
       *  @brief  シェーダーを開放する
       */
      virtual void Release() noexcept = 0;

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
       *  @brief  シェーダーを作成する
       *  @param  shader_file_path:シェーダーファイルのパス
       *  @return シェーダーへのシェアードポインタ
       */
      static std::shared_ptr<IShader> Create(const std::string& shader_file_path);
    };
  };
};