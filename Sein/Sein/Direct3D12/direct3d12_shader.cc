/**
 *  @file     shader.cc
 *  @brief    �V�F�[�_�[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/20
 *  @version  1.0
 */

 // include
#include <fstream>
#include "direct3d12_shader.h"

namespace Sein
{
  namespace Direct3D12
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
        Shader() : file_path_(), shader_code_(), bytecode_({})
        {

        }
        
        /**
         *  @brief  �f�X�g���N�^
         */
        ~Shader() override
        {
          Release();
        }

        /**
         *  @brief  �V�F�[�_�[���쐬����
         *  @param  file_path:�V�F�[�_�[�t�@�C���p�X
         *  @param  shader_bytecode:�V�F�[�_�[�o�C�g�R�[�h
         */
        void Create(const std::string& file_path, const std::string& shader_bytecode)
        {
          file_path_ = file_path;
          shader_code_ = shader_bytecode;

          bytecode_.pShaderBytecode = &(shader_code_[0]);
          bytecode_.BytecodeLength = shader_code_.size();
        }

        /**
         *  @brief  �V�F�[�_�[���擾����
         *  @return �V�F�[�_�[
         */
        const D3D12_SHADER_BYTECODE& Get() const override
        {
          return bytecode_;
        }
        
        /**
         *  @brief  �V�F�[�_�[���J������
         */
        void Release() noexcept override
        {
          file_path_.clear();
          shader_code_.clear();
          bytecode_ = {};
        }

      private:
        std::string file_path_;           ///< �t�@�C���p�X
        std::string shader_code_;         ///< �V�F�[�_�[�R�[�h
        D3D12_SHADER_BYTECODE bytecode_;  ///< �V�F�[�_�[�o�C�g�R�[�h
      };
    };

    /**
     *  @brief  �V�F�[�_�[���쐬����
     *  @param  shader_file_path:�V�F�[�_�[�t�@�C���̃p�X
     *  @return �V�F�[�_�[�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IShader> IShader::Create(const std::string& shader_file_path)
    {
      std::ifstream stream;
      stream.open(shader_file_path, std::ifstream::in | std::ifstream::binary);
      if (!stream)
      {
        throw std::exception("file open failed");
      }

      // �ǂݍ��ݎ��s
      std::istreambuf_iterator<char> it(stream);
      std::istreambuf_iterator<char> last;
      std::string data(it, last);
      stream.close();

      D3D12_SHADER_BYTECODE bytecode = {};
      bytecode.pShaderBytecode = &(data);
      bytecode.BytecodeLength = data.size();

      auto shader = std::make_shared<Shader>();
      shader->Create(shader_file_path, data);
      return shader;
    }
  };
};