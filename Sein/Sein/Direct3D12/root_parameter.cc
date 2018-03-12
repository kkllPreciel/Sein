/**
 *  @file     root_parameter.cc
 *  @brief    ルートパラメータに関する処理を行うプログラムソース
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
         *  @brief  コンストラクタ
         */
        RootParameter() : root_parameter_({})
        {

        }

        /**
         *  @brief  デストラクタ
         */
        ~RootParameter() override
        {

        }

        /**
         *  @brief  パラメータ種別を設定する
         *  @param  parameter_type:パラメータ種別
         */
        void SetParameterType(const D3D12_ROOT_PARAMETER_TYPE parameter_type)
        {
          root_parameter_.ParameterType = parameter_type;
        }

        /**
         *  @brief  ディスクリプターレンジの個数を設定する
         *  @param  descriptor_range_count:ディスクリプターレンジの個数
         */
        void SetDescriptorRangeCount(const std::uint32_t descriptor_range_count)
        {
          root_parameter_.DescriptorTable.NumDescriptorRanges = descriptor_range_count;
        }

        /**
         *  @brief  ディスクリプターレンジを設定する
         *  @param  descriptor_ranges:ディスクリプターレンジへのポインタ
         */
        void SetDescriptorRanges(const D3D12_DESCRIPTOR_RANGE* descriptor_ranges)
        {
          root_parameter_.DescriptorTable.pDescriptorRanges = descriptor_ranges;
        }

        /**
         *  @brief  アクセス可能なシェーダー種別を設定する
         *  @param  shader_visibility:ルートシグネチャのスロットの内容にアクセスできるシェーダーの種別
         */
        void SetShaderVisibility(const D3D12_SHADER_VISIBILITY shader_visibility)
        {
          root_parameter_.ShaderVisibility = shader_visibility;
        }

        /**
         *  @brief  ルートパラメータを取得する
         *  @return ルートパラメータ
         */
        const D3D12_ROOT_PARAMETER& Get() const override
        {
          return root_parameter_;
        }

      private:
        D3D12_ROOT_PARAMETER root_parameter_; ///< ルートパラメータ
      };
    };

    /**
     *  @brief  タイプがディスクリプターテーブルのルートパラメータを作成する
     *  @param  descriptor_range_type:ディスクリプターレンジの種別
     *  @param  descriptor_count:ディスクリプターの個数
     *  @return ディスクリプターレンジインターフェイスへのシェアードポインタ
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