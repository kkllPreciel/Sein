/**
 *  @file     rasterizer_desc.cc
 *  @brief    ���X�^���C�U�̐ݒ�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/23
 *  @version  1.0
 */

 // include
#include "direct3d12_rasterizer_desc.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �f�t�H���g�ݒ�̃��X�^���C�U�̐ݒ���쐬����
     *  @param  fill_mode:�t�B�����[�h(�f�t�H���g�͓h��Ԃ�)
     *  @param  cull_mode:�J�����O�̃��[�h(�f�t�H���g�̓J�����O�Ȃ�)
     *  @return ���X�^���C�U�̐ݒ�
     */
    D3D12_RASTERIZER_DESC CreateDefaultRasterizerDesc(D3D12_FILL_MODE fill_mode, D3D12_CULL_MODE cull_mode)
    {
      // ���X�^���C�U�[�X�e�[�g�̐ݒ�
      D3D12_RASTERIZER_DESC desc;
      desc.FillMode = fill_mode;                                           // �O�p�`��`�悷��Ƃ��Ɏg�p����h��Ԃ����[�h(����͓h��Ԃ�)
      desc.CullMode = cull_mode;                                           // �J�����O�̃��[�h(�������̃|���S����`�悵�Ȃ�)
      desc.FrontCounterClockwise = FALSE;                                  // �|���S���̕\���̔�����@(����͎��v���Ȃ�\)
      desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // �[�x�o�C�A�X(Z-Fighting�΍�?)
      desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // �[�x�o�C�A�X�̃N�����v�l
      desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // �X�΂��l�������[�x�o�C�A�X(�s�[�^�[�p�����ۑ΍�?)
      desc.DepthClipEnable = TRUE;                                         // Z�N���b�s���O���s����(����͍s��)
      desc.MultisampleEnable = FALSE;                                      // �}���`�T���v�����O�̃A���`�G�C���A�V���O���s����(����͂��Ȃ�)
      desc.AntialiasedLineEnable = FALSE;                                  // ���̃A���`�G�C���A�V���O���s����(����͂��Ȃ�)
      desc.ForcedSampleCount = 0;                                          // UAV�����_�����O�܂��̓��X�^���C�Y���ɋ��������T���v����(����͋������Ȃ�)
      desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // �ێ�I�ȃ��X�^���C�Y�t���O(����̓I�t)
      return desc;
    }
  };
};