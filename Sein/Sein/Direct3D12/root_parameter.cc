/**
 *  @file     root_parameter.cc
 *  @brief    ���[�g�p�����[�^�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/11
 *  @version  1.0
 */

 // include
#include "root_parameter.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      class RootParameter final : public IRootParameter
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        RootParameter() : root_parameter_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~RootParameter() override
        {

        }

        /**
         *  @brief  ���[�g�p�����[�^���擾����
         *  @return ���[�g�p�����[�^
         */
        const D3D12_ROOT_PARAMETER& Get() const override
        {
          return root_parameter_;
        }

      private:
        D3D12_ROOT_PARAMETER root_parameter_; ///< ���[�g�p�����[�^
      };
    };

    /**
     *  @brief  �^�C�v���f�B�X�N���v�^�[�e�[�u���̃��[�g�p�����[�^���쐬����
     *  @param  descriptor_range_type:�f�B�X�N���v�^�[�����W�̎��
     *  @param  descriptor_count:�f�B�X�N���v�^�[�̌�
     *  @return �f�B�X�N���v�^�[�����W�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IRootParameter> IRootParameter::CreateForDescriptorTable(const IDescriptorRange& descriptor_range, const D3D12_SHADER_VISIBILITY& shader_visibility)
    {
      auto root_parameter = std::make_unique<RootParameter>();

      return root_parameter;
    }
  };
};