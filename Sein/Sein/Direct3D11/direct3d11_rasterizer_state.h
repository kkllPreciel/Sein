/**
 *  @file     direct3d11_rasterizer_state.h
 *  @brief    Direct3D11�̃��X�^���C�U�X�e�[�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/12
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
     *  @brief  Direct3D11�̃��X�^���C�U�X�e�[�g�p�C���^�[�t�F�[�X
     */
    class IRasterizerState
    {
    public:
      /**
       *  @brief  �h��Ԃ��̎��
       */
      enum class FillMode : std::uint32_t {
        kUndefined, ///< ����`
        kWireframe, ///< ���C���[�t���[��
        kSolid,     ///< �h��Ԃ�
      };

      /**
       *  @brief  �J�����O�̎��
       */
      enum class CullMode : std::uint32_t {
        kUndefined, ///< ����`
        kNone,      ///< �J�����O����
        kFront,     ///< �\�ʂ��J�����O����
        kBack,      ///< ���ʂ��J�����O����
      };

      /**
       *  @brief  ���X�^���C�U�̐ݒ�
       */
      class Desc
      {
      public:
        FillMode fill_mode = FillMode::kSolid;      ///< �h��Ԃ��̎��
        CullMode cull_mode = CullMode::kBack;       ///< �J�����O�̎��
        bool front_counter_clockwise = false;       ///< �����v���(������)�̖ʂ�\�Ƃ���t���O
        std::int32_t depth_bias = 0;                ///< �w��̃s�N�Z���ɉ��Z����o�C�A�X�l
        std::float_t depth_bias_clamp = 0.f;        ///< �s�N�Z���̍ő�[�x�o�C�A�X�l
        std::float_t slope_scaled_depth_bias = 0.f; ///< �w��̃s�N�Z���̃X���[�v�ɑ΂���X�J��
        bool depth_clip_enable = true;              ///< �����Ɋ�Â����N���b�s���O�L���t���O
        bool scissor_enable = false;                ///< �V�U�[�e�X�g�L���t���O
        bool multisample_enable = false;            ///< �}���`�T���v�����O�̃A���`�G�C���A�V���O�L���t���O
        bool antialiased_line_enable = false;       ///< ���̃A���`�G�C���A�V���O�L���t���O(multisample_enable�������̏ꍇ�̂ݓK�p)
      };

      /**
       *  @brief  �R���X�g���N�^
       */
      IRasterizerState() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IRasterizerState() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IRasterizerState(const IRasterizerState& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IRasterizerState& operator = (const IRasterizerState& other) = delete;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  ���X�^���C�U�X�e�[�g���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  desc:���X�^���C�U�̐ݒ�
       *  @return ���X�^���C�U�X�e�[�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IRasterizerState> Create(ID3D11Device* const device, const Desc& desc);
    };
  };
};