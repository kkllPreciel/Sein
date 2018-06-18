/**
 *  @file     direct3d11_input_layout.cc
 *  @brief    Direct3D11の入力レイアウトに関する処理を行うプログラムソース
 *  @author   kkllPreciel
 *  @date     2018/06/18
 *  @version  1.0
 */

 // include
#include "direct3d11_input_layout.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  入力レイアウト用クラス
       */
      class InputLayout final : public IInputLayout
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        InputLayout() : input_layout_(nullptr, [](ID3D11InputLayout* p) { p->Release(); }), input_layout_desc_list_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~InputLayout() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  input_layout_desc_list:入力レイアウトの設定
         */
        void Create(ID3D11Device* const device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc_list)
        {
          ID3D11InputLayout* input_layout;
          if (FAILED(device->CreateInputLayout(input_layout_desc_list.data(), input_layout_desc_list.size(), nullptr, nullptr, &input_layout)))
          {
            throw std::exception("インプットレイアウトの作成に失敗しました。");
          }

          input_layout_.reset(input_layout);
          input_layout_desc_list_ = input_layout_desc_list;
        }
        
        /**
         *  @brief  終了処理を行う
         */
        void Destroy() override
        {
          input_layout_.reset();
        }

      private:
        std::unique_ptr<ID3D11InputLayout, std::function<void(ID3D11InputLayout*)>> input_layout_;  ///< 入力レイアウト
        std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_desc_list_;                              ///< 入力レイアウトの設定
      };
    };

    /**
     *  @brief  入力レイアウトを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  input_layout_desc_list:入力レイアウトの設定
     *  @return 入力レイアウトへのシェアードポインタ
     */
    std::shared_ptr<IInputLayout> IInputLayout::Create(ID3D11Device* const device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc_list)
    {
      auto input_layout = std::make_shared<InputLayout>();

      input_layout->Create(device, input_layout_desc_list);

      return input_layout;
    }
  };
};