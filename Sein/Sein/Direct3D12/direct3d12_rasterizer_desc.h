/**
 *  @file     rasterizer_desc.h
 *  @brief    ���X�^���C�U�̐ݒ�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/23
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>

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
    D3D12_RASTERIZER_DESC CreateDefaultRasterizerDesc(D3D12_FILL_MODE fill_mode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cull_mode = D3D12_CULL_MODE_NONE);
  };
};