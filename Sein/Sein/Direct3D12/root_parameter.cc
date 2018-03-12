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
         *  @brief  �p�����[�^��ʂ�ݒ肷��
         *  @param  parameter_type:�p�����[�^���
         */
        void SetParameterType(const D3D12_ROOT_PARAMETER_TYPE parameter_type)
        {
          root_parameter_.ParameterType = parameter_type;
        }

        /**
         *  @brief  �f�B�X�N���v�^�[�����W�̌���ݒ肷��
         *  @param  descriptor_range_count:�f�B�X�N���v�^�[�����W�̌�
         */
        void SetDescriptorRangeCount(const std::uint32_t descriptor_range_count)
        {
          root_parameter_.DescriptorTable.NumDescriptorRanges = descriptor_range_count;
        }

        /**
         *  @brief  �f�B�X�N���v�^�[�����W��ݒ肷��
         *  @param  descriptor_ranges:�f�B�X�N���v�^�[�����W�ւ̃|�C���^
         */
        void SetDescriptorRanges(const D3D12_DESCRIPTOR_RANGE* descriptor_ranges)
        {
          root_parameter_.DescriptorTable.pDescriptorRanges = descriptor_ranges;
        }

        /**
         *  @brief  �A�N�Z�X�\�ȃV�F�[�_�[��ʂ�ݒ肷��
         *  @param  shader_visibility:���[�g�V�O�l�`���̃X���b�g�̓��e�ɃA�N�Z�X�ł���V�F�[�_�[�̎��
         */
        void SetShaderVisibility(const D3D12_SHADER_VISIBILITY shader_visibility)
        {
          root_parameter_.ShaderVisibility = shader_visibility;
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
      root_parameter->SetParameterType(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);
      root_parameter->SetDescriptorRangeCount(1);
      root_parameter->SetDescriptorRanges(&(descriptor_range.Get()));
      root_parameter->SetShaderVisibility(shader_visibility);

      return root_parameter;
    }
  };
};