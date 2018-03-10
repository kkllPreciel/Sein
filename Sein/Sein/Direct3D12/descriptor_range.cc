/**
 *  @file     descriptor_range.cc
 *  @brief    �f�B�X�N���v�^�[�����W�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/10
 *  @version  1.0
 */

 // include
#include "descriptor_range.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �f�B�X�N���v�^�[�����W�p�N���X
       */
      class DescriptorRange final : public IDescriptorRange
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        DescriptorRange() : descriptor_range_({})
        {
          descriptor_range_.BaseShaderRegister = 0;
          descriptor_range_.RegisterSpace = 0;
          descriptor_range_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~DescriptorRange() override
        {

        }

        /**
         *  @brief  �f�B�X�N���v�^�[�����W�̎�ʂ�ݒ肷��
         *  @param  descriptor_range_type:�f�B�X�N���v�^�[�����W�̎��
         */
        void SetDescriptorRangeType(const D3D12_DESCRIPTOR_RANGE_TYPE descriptor_range_type)
        {
          descriptor_range_.RangeType = descriptor_range_type;
        }

        /**
         *  @brief  �f�B�X�N���v�^�[�̌���ݒ肷��
         *  @param  descriptor_count:�f�B�X�N���v�^�[�̌�
         */
        void SetDescriptorCount(const std::uint32_t descriptor_count)
        {
          descriptor_range_.NumDescriptors = descriptor_count;
        }

        /**
         *  @brief  �V�F�[�_�[���W�X�^�[�̊J�n�ԍ���ݒ肷��
         *  @param  shader_register_index:�V�F�[�_�[���W�X�^�[�J�n�ԍ�
         */
        void SetShaderRegisterIndex(const std::uint32_t shader_register_index)
        {
          descriptor_range_.BaseShaderRegister = shader_register_index;
        }

        /**
         *  @brief  �V�F�[�_�[���W�X�^�[�̃X�y�[�X��ݒ肷��
         *  @param  shader_register_space:�V�F�[�_�[���W�X�^�[�̃X�y�[�X
         */
        void SetShaderRegisterSpace(const std::uint32_t shader_register_space)
        {
          descriptor_range_.RegisterSpace = shader_register_space;
        }

        /**
         *  @brief  �擪����̃f�B�X�N���v�^�[���̃I�t�Z�b�g��ݒ肷��
         *  @param  descriptor_offset:�I�t�Z�b�g
         */
        void SetDescriptorOffset(const std::uint32_t descriptor_offset)
        {
          descriptor_range_.OffsetInDescriptorsFromTableStart = descriptor_offset;
        }

        /**
         *  @brief  �f�B�X�N���v�^�[�����W���擾����
         *  @return �f�B�X�N���v�^�[�����W
         */
        D3D12_DESCRIPTOR_RANGE Get() override
        {
          return descriptor_range_;
        }

      private:
        D3D12_DESCRIPTOR_RANGE descriptor_range_; ///< �f�B�X�N���v�^�[�����W
      };
    };

    /**
     *  @brief  �f�B�X�N���v�^�[�����W���쐬����
     *  @param  descriptor_range_type:�f�B�X�N���v�^�[�����W�̎��
     *  @param  descriptor_count:�f�B�X�N���v�^�[�̌�
     *  @return �f�B�X�N���v�^�[�����W�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IDescriptorRange> IDescriptorRange::Create(const D3D12_DESCRIPTOR_RANGE_TYPE descriptor_range_type, std::uint32_t descriptor_count)
    {
      auto descriptor_range = std::make_shared<DescriptorRange>();

      descriptor_range->SetDescriptorRangeType(descriptor_range_type);
      descriptor_range->SetDescriptorCount(descriptor_count);
      descriptor_range->SetShaderRegisterIndex(0);
      descriptor_range->SetShaderRegisterSpace(0);
      descriptor_range->SetDescriptorOffset(D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND);

      return descriptor_range;
    }
  };
};