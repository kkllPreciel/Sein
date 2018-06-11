/**
 *  @file     direct3d11_shader.cc
 *  @brief    Direct3D11のシェーダーに関する処理を行うプログラムソース
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
       *  @brief  シェーダー用クラス
       */
      class Shader final : public IShader
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        Shader() : vertex_shader_(nullptr, [](ID3D11VertexShader* p) { p->Release(); }), pixel_shader_(nullptr, [](ID3D11PixelShader* p) { p->Release(); }),
          geometry_shader_(nullptr, [](ID3D11GeometryShader* p) { p->Release(); }), domain_shader_(nullptr, [](ID3D11DomainShader* p) { p->Release(); }),
          hull_shader_(nullptr, [](ID3D11HullShader* p) { p->Release(); }), compute_shader_(nullptr, [](ID3D11ComputeShader* p) { p->Release(); })
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~Shader() override
        {
          Destroy();
        }

        /**
         *  @brief  作成する
         *  @param  device:Direct3D11のデバイス
         *  @param  shader_desc:シェーダーの設定
         */
        void Create(ID3D11Device* const device, const Desc& shader_desc)
        {
          // TODO:template,SFINAEを使って簡単に記述できないか調べる
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
            // throw std::exception("違法なシェーダー種別です。:" + std::to_string(shader_desc.shader_type));
            throw std::exception("違法なシェーダー種別です。");
            break;
          }

          if (FAILED(hr))
          {
            // throw std::exception("シェーダーの作成に失敗しました。:" + std::to_string(shader_desc.shader_type));
            throw std::exception("シェーダーの作成に失敗しました。");
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
         *  @brief  終了処理を行う
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
        Desc desc_;                                                                                         ///< シェーダーの設定
        std::unique_ptr<ID3D11VertexShader, std::function<void(ID3D11VertexShader*)>>     vertex_shader_;   ///< 頂点シェーダー
        std::unique_ptr<ID3D11PixelShader, std::function<void(ID3D11PixelShader*)>>       pixel_shader_;    ///< ピクセル(フラグメント)シェーダー
        std::unique_ptr<ID3D11GeometryShader, std::function<void(ID3D11GeometryShader*)>> geometry_shader_; ///< ジオメトリシェーダー
        std::unique_ptr<ID3D11DomainShader, std::function<void(ID3D11DomainShader*)>>     domain_shader_;   ///< ドメインシェーダー
        std::unique_ptr<ID3D11HullShader, std::function<void(ID3D11HullShader*)>>         hull_shader_;     ///< ハルシェーダー
        std::unique_ptr<ID3D11ComputeShader, std::function<void(ID3D11ComputeShader*)>>   compute_shader_;  ///< コンピュートシェーダー
      };
    };

    /**
     *  @brief  シェーダーを作成する
     *  @param  device:Direct3D11のデバイス
     *  @param  shader_desc:シェーダーの設定
     *  @return シェーダーへのシェアードポインタ
     */
    std::shared_ptr<IShader> IShader::Create(ID3D11Device* const device, const Desc& shader_desc)
    {
      auto shader = std::make_shared<Shader>();

      shader->Create(device, shader_desc);

      return shader;
    }
  };
};