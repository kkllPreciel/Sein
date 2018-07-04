/**
 *  @file     shader.cc
 *  @brief    シェーダーに関する処理を行うプログラムソース
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
       *  @brief  シェーダー用クラス
       */
      class Shader final : public IShader
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Shader() : file_path_(), shader_code_(), bytecode_({})
        {

        }
        
        /**
         *  @brief  デストラクタ
         */
        ~Shader() override
        {
          Release();
        }

        /**
         *  @brief  シェーダーを作成する
         *  @param  file_path:シェーダーファイルパス
         *  @param  shader_bytecode:シェーダーバイトコード
         */
        void Create(const std::string& file_path, const std::string& shader_bytecode)
        {
          file_path_ = file_path;
          shader_code_ = shader_bytecode;

          bytecode_.pShaderBytecode = &(shader_code_[0]);
          bytecode_.BytecodeLength = shader_code_.size();
        }

        /**
         *  @brief  シェーダーを取得する
         *  @return シェーダー
         */
        const D3D12_SHADER_BYTECODE& Get() const override
        {
          return bytecode_;
        }
        
        /**
         *  @brief  シェーダーを開放する
         */
        void Release() noexcept override
        {
          file_path_.clear();
          shader_code_.clear();
          bytecode_ = {};
        }

      private:
        std::string file_path_;           ///< ファイルパス
        std::string shader_code_;         ///< シェーダーコード
        D3D12_SHADER_BYTECODE bytecode_;  ///< シェーダーバイトコード
      };
    };

    /**
     *  @brief  シェーダーを作成する
     *  @param  shader_file_path:シェーダーファイルのパス
     *  @return シェーダーへのシェアードポインタ
     */
    std::shared_ptr<IShader> IShader::Create(const std::string& shader_file_path)
    {
      std::ifstream stream;
      stream.open(shader_file_path, std::ifstream::in | std::ifstream::binary);
      if (!stream)
      {
        throw std::exception("file open failed");
      }

      // 読み込み実行
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