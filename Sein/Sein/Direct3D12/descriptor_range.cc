/**
 *  @file     descriptor_range.cc
 *  @brief    ディスクリプターレンジに関する処理を行うプログラムソース
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
       *  @brief  ディスクリプターレンジ用クラス
       */
      class DescriptorRange final : public IDescriptorRange
      {
      public:
        /**
         *  @brief  コンストラクタ
         */
        DescriptorRange() : descriptor_range_({})
        {
          descriptor_range_.BaseShaderRegister = 0;
          descriptor_range_.RegisterSpace = 0;
          descriptor_range_.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        }

        /**
         *  @brief  デストラクタ
         */
        ~DescriptorRange() override
        {

        }

        /**
         *  @brief  ディスクリプターレンジの種別を設定する
         *  @param  descriptor_range_type:ディスクリプターレンジの種別
         */
        void SetDescriptorRangeType(const D3D12_DESCRIPTOR_RANGE_TYPE descriptor_range_type)
        {
          descriptor_range_.RangeType = descriptor_range_type;
        }

        /**
         *  @brief  ディスクリプターの個数を設定する
         *  @param  descriptor_count:ディスクリプターの個数
         */
        void SetDescriptorCount(const std::uint32_t descriptor_count)
        {
          descriptor_range_.NumDescriptors = descriptor_count;
        }

        /**
         *  @brief  シェーダーレジスターの開始番号を設定する
         *  @param  shader_register_index:シェーダーレジスター開始番号
         */
        void SetShaderRegisterIndex(const std::uint32_t shader_register_index)
        {
          descriptor_range_.BaseShaderRegister = shader_register_index;
        }

        /**
         *  @brief  シェーダーレジスターのスペースを設定する
         *  @param  shader_register_space:シェーダーレジスターのスペース
         */
        void SetShaderRegisterSpace(const std::uint32_t shader_register_space)
        {
          descriptor_range_.RegisterSpace = shader_register_space;
        }

        /**
         *  @brief  先頭からのディスクリプター内のオフセットを設定する
         *  @param  descriptor_offset:オフセット
         */
        void SetDescriptorOffset(const std::uint32_t descriptor_offset)
        {
          descriptor_range_.OffsetInDescriptorsFromTableStart = descriptor_offset;
        }

        /**
         *  @brief  ディスクリプターレンジを取得する
         *  @return ディスクリプターレンジ
         */
        D3D12_DESCRIPTOR_RANGE Get() override
        {
          return descriptor_range_;
        }

      private:
        D3D12_DESCRIPTOR_RANGE descriptor_range_; ///< ディスクリプターレンジ
      };
    };

    /**
     *  @brief  ディスクリプターレンジを作成する
     *  @param  descriptor_range_type:ディスクリプターレンジの種別
     *  @param  descriptor_count:ディスクリプターの個数
     *  @return ディスクリプターレンジインターフェイスへのシェアードポインタ
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