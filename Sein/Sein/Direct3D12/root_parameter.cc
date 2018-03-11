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

      return root_parameter;
    }
  };
};