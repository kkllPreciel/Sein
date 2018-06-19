/**
 *  @file     direct3d11_input_layout.h
 *  @brief    Direct3D11の入力レイアウトに関するヘッダファイル
 *  @author   kkllPreciel
 *  @date     2018/06/18
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include <d3d11.h>

#include "direct3d11_shader.h"

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11の入力レイアウト用インターフェース
     */
    class IInputLayout
    {
    public:
      /**
       *  @brief  コンストラクタ
       */
      IInputLayout() = default;
      
      /**
       *  @brief  デストラクタ
       */
      virtual ~IInputLayout() = default;
      
      /**
       *  @brief  コピーコンストラクタ
       *  @param  other:コピー元のインスタンス
       */
      IInputLayout(const IInputLayout& other) = delete;
      
      /**
       *  @brief  代入演算子オペレータ
       *  @param  other:代入元のインスタンス
       *  @return 代入後のインスタンス
       */
      IInputLayout& operator = (const IInputLayout& other) = delete;
      
      /**
       *  @brief  終了処理を行う
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  入力レイアウトを作成する
       *  @param  device:Direct3D11のデバイス
       *  @param  input_layout_desc_list:入力レイアウトの設定
       *  @param  shader_desc:シェーダーの設定
       *  @return 入力レイアウトへのシェアードポインタ
       */
      static std::shared_ptr<IInputLayout> Create(ID3D11Device* const device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc_list, const IShader::Desc& shader_desc);
    };
  };
};