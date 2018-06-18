/**
 *  @file     direct3d11_input_layout.cc
 *  @brief    Direct3D11�̓��̓��C�A�E�g�Ɋւ��鏈�����s���v���O�����\�[�X
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
       *  @brief  ���̓��C�A�E�g�p�N���X
       */
      class InputLayout final : public IInputLayout
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        InputLayout() : input_layout_(nullptr, [](ID3D11InputLayout* p) { p->Release(); }), input_layout_desc_list_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~InputLayout() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  input_layout_desc_list:���̓��C�A�E�g�̐ݒ�
         */
        void Create(ID3D11Device* const device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc_list)
        {
          ID3D11InputLayout* input_layout;
          if (FAILED(device->CreateInputLayout(input_layout_desc_list.data(), input_layout_desc_list.size(), nullptr, nullptr, &input_layout)))
          {
            throw std::exception("�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂����B");
          }

          input_layout_.reset(input_layout);
          input_layout_desc_list_ = input_layout_desc_list;
        }
        
        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          input_layout_.reset();
        }

      private:
        std::unique_ptr<ID3D11InputLayout, std::function<void(ID3D11InputLayout*)>> input_layout_;  ///< ���̓��C�A�E�g
        std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout_desc_list_;                              ///< ���̓��C�A�E�g�̐ݒ�
      };
    };

    /**
     *  @brief  ���̓��C�A�E�g���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  input_layout_desc_list:���̓��C�A�E�g�̐ݒ�
     *  @return ���̓��C�A�E�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IInputLayout> IInputLayout::Create(ID3D11Device* const device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc_list)
    {
      auto input_layout = std::make_shared<InputLayout>();

      input_layout->Create(device, input_layout_desc_list);

      return input_layout;
    }
  };
};