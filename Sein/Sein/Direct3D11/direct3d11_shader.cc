/**
 *  @file     direct3d11_shader.cc
 *  @brief    Direct3D11�̃V�F�[�_�[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/11
 *  @version  1.0
 */

 // include
#include "direct3d11_shader.h"
#include <functional>
#include <string>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �V�F�[�_�[�p�N���X
       */
      class Shader final : public IShader
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Shader() : vertex_shader_(nullptr, [](ID3D11VertexShader* p) { p->Release(); }), pixel_shader_(nullptr, [](ID3D11PixelShader* p) { p->Release(); }),
          geometry_shader_(nullptr, [](ID3D11GeometryShader* p) { p->Release(); }), domain_shader_(nullptr, [](ID3D11DomainShader* p) { p->Release(); }),
          hull_shader_(nullptr, [](ID3D11HullShader* p) { p->Release(); }), compute_shader_(nullptr, [](ID3D11ComputeShader* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Shader() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  shader_desc:�V�F�[�_�[�̐ݒ�
         */
        void Create(ID3D11Device* const device, const Desc& shader_desc)
        {
          // TODO:template,SFINAE���g���ĊȒP�ɋL�q�ł��Ȃ������ׂ�
          ID3D11VertexShader* vertex_shader = nullptr;
          ID3D11PixelShader* pixel_shader = nullptr;
          ID3D11GeometryShader* geometry_shader = nullptr;
          ID3D11DomainShader* domain_shader = nullptr;
          ID3D11HullShader* hull_shader = nullptr;
          ID3D11ComputeShader* compute_shader = nullptr;
          HRESULT hr;

          switch (shader_desc.shader_type)
          {
          case Type::Vertex:
            hr = device->CreateVertexShader(shader_desc.shader_bytecode, shader_desc.bytecode_size, nullptr, &vertex_shader);
            break;
          case Type::Pixel:
            hr = device->CreatePixelShader(shader_desc.shader_bytecode, shader_desc.bytecode_size, nullptr, &pixel_shader);
            break;
          case Type::Geometry:
            hr = device->CreateGeometryShader(shader_desc.shader_bytecode, shader_desc.bytecode_size, nullptr, &geometry_shader);
            break;
          case Type::Domain:
            hr = device->CreateDomainShader(shader_desc.shader_bytecode, shader_desc.bytecode_size, nullptr, &domain_shader);
            break;
          case Type::Hull:
            hr = device->CreateHullShader(shader_desc.shader_bytecode, shader_desc.bytecode_size, nullptr, &hull_shader);
            break;
          case Type::Compute:
            hr = device->CreateComputeShader(shader_desc.shader_bytecode, shader_desc.bytecode_size, nullptr, &compute_shader);
            break;
          default:
            // throw std::exception("��@�ȃV�F�[�_�[��ʂł��B:" + std::to_string(shader_desc.shader_type));
            throw std::exception("��@�ȃV�F�[�_�[��ʂł��B");
            break;
          }

          if (FAILED(hr))
          {
            // throw std::exception("�V�F�[�_�[�̍쐬�Ɏ��s���܂����B:" + std::to_string(shader_desc.shader_type));
            throw std::exception("�V�F�[�_�[�̍쐬�Ɏ��s���܂����B");
          }

          vertex_shader_.reset(vertex_shader);
          pixel_shader_.reset(pixel_shader);
          geometry_shader_.reset(geometry_shader);
          domain_shader_.reset(domain_shader);
          hull_shader_.reset(hull_shader);
          compute_shader_.reset(compute_shader);
          desc_ = shader_desc;
        }

        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          vertex_shader_.reset();
          pixel_shader_.reset();
          geometry_shader_.reset();
          domain_shader_.reset();
          hull_shader_.reset();
          compute_shader_.reset();
        }

      private:
        Desc desc_;                                                                                         ///< �V�F�[�_�[�̐ݒ�
        std::unique_ptr<ID3D11VertexShader, std::function<void(ID3D11VertexShader*)>>     vertex_shader_;   ///< ���_�V�F�[�_�[
        std::unique_ptr<ID3D11PixelShader, std::function<void(ID3D11PixelShader*)>>       pixel_shader_;    ///< �s�N�Z��(�t���O�����g)�V�F�[�_�[
        std::unique_ptr<ID3D11GeometryShader, std::function<void(ID3D11GeometryShader*)>> geometry_shader_; ///< �W�I���g���V�F�[�_�[
        std::unique_ptr<ID3D11DomainShader, std::function<void(ID3D11DomainShader*)>>     domain_shader_;   ///< �h���C���V�F�[�_�[
        std::unique_ptr<ID3D11HullShader, std::function<void(ID3D11HullShader*)>>         hull_shader_;     ///< �n���V�F�[�_�[
        std::unique_ptr<ID3D11ComputeShader, std::function<void(ID3D11ComputeShader*)>>   compute_shader_;  ///< �R���s���[�g�V�F�[�_�[
      };
    };

    /**
     *  @brief  �V�F�[�_�[���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  shader_desc:�V�F�[�_�[�̐ݒ�
     *  @return �V�F�[�_�[�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IShader> IShader::Create(ID3D11Device* const device, const Desc& shader_desc)
    {
      auto shader = std::make_shared<Shader>();

      shader->Create(device, shader_desc);

      return shader;
    }
  };
};