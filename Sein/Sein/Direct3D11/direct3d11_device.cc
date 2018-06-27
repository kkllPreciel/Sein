/**
 *  @file     direct3d11_device.cc
 *  @brief    Direct3D11�̃f�o�C�X�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/05/22
 *  @version  1.0
 */

 // include
#include "direct3d11_device.h"
#include <cassert>
#include <array>
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �f�o�C�X�p�N���X
       */
      class Device final : public IDevice
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Device()
          : device_(nullptr, [](ID3D11Device* p) { p->Release(); }),
          immediate_context_(nullptr, [](ID3D11DeviceContext* p) { p->Release(); }),
          swap_chain_(nullptr, [](IDXGISwapChain* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Device() override
        {
          Destroy();
        }

        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          swap_chain_.reset();
          immediate_context_.reset();
          device_.reset();
        }

        /**
         *  @brief  �쐬����
         *  @param  handle:�E�B���h�E�n���h��
         *  @param  width:�E�B���h�E�̉���
         *  @param  height:�E�B���h�E�̏c��
         */
        void Create(const HWND& handle, const std::uint32_t& width, const std::uint32_t& height)
        {
          assert(device_ == nullptr);

          UINT createDeviceFlags = 0;
#ifdef _DEBUG
          createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
          // �h���C�o�[���
          std::array<D3D_DRIVER_TYPE, 3> driver_types = {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
          };

          // �@�\���x��
          std::array<D3D_FEATURE_LEVEL, 2> feature_levels = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
          };

          // �X���b�v�`�F�C���̐ݒ�
          DXGI_SWAP_CHAIN_DESC swap_chain_desc = {0};
          swap_chain_desc.BufferCount = 1;
          swap_chain_desc.BufferDesc.Width = width;
          swap_chain_desc.BufferDesc.Height = height;
          swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
          swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
          swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
          swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
          swap_chain_desc.OutputWindow = handle;
          swap_chain_desc.SampleDesc.Count = 1;
          swap_chain_desc.SampleDesc.Quality = 0;
          swap_chain_desc.Windowed = TRUE;

          for (auto driver_type : driver_types)
          {
            ID3D11Device* device;
            ID3D11DeviceContext* context;
            IDXGISwapChain* swap_chain;

            // �f�o�C�X�ƃX���b�v�`�F�C�����쐬����
            // �f�o�C�X�݂̂ō쐬���邱�Ƃ��\
            // ���̏ꍇ�A�X���b�v�`�F�C���͌�ō쐬����
            if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(
              nullptr,
              driver_type,
              nullptr,
              createDeviceFlags,
              feature_levels.data(),
              static_cast<UINT>(feature_levels.size()),
              D3D11_SDK_VERSION,
              &swap_chain_desc,
              &swap_chain,
              &device,
              &feature_level_,
              &context)))
            {
              device_.reset(device);
              immediate_context_.reset(context);
              swap_chain_.reset(swap_chain);
              driver_type_ = driver_type;

              break;
            }
          }

          if (!device_ || !immediate_context_ || !swap_chain_)
          {
            throw std::exception("�f�o�C�X�̐����Ɏ��s���܂����B");
          }
        }

        /**
         *  @brief  �f�o�C�X�R���e�L�X�g���쐬����
         *  @param  context_type:�f�o�C�X�R���e�L�X�g�̎��
         *  @return �f�o�C�X�R���e�L�X�g�p�C���^�[�t�F�[�X�ւ̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<IDeviceContext> CreateDeviceContext(const IDeviceContext::Type& context_type) override
        {
          return IDeviceContext::Create(device_.get(), context_type);
        }

        /**
         *  @brief  �p�C�v���C���X�e�[�g���쐬����
         *  @param  pipeline_desc:�p�C�v���C���̐ݒ�
         *  @return �p�C�v���C���X�e�[�g�p�C���^�[�t�F�[�X�ւ̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<IPipelineState> CreatePipelineState(const IPipelineState::Desc& pipeline_desc) override
        {
          return IPipelineState::Create(device_.get(), pipeline_desc);
        }

        /**
         *  @brief  �V�F�[�_�[���쐬����
         *  @param  shader_desc:�V�F�[�_�[�̐ݒ�
         *  @return �V�F�[�_�[�p�C���^�[�t�F�[�X�ւ̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<IShader> CreateShader(const IShader::Desc& shader_desc) override
        {
          return IShader::Create(device_.get(), shader_desc);
        }

        /**
         *  @brief  ���_�o�b�t�@���쐬����
         *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
         *  @return ���_�o�b�t�@�ւ̃X�}�[�g�|�C���^
         */
        std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t& size_in_bytes) override
        {
          return IVertexBuffer::Create(device_.get(), size_in_bytes);
        }

      private:
        std::unique_ptr<ID3D11Device, std::function<void(ID3D11Device*)>> device_;                          ///< �f�o�C�X
        std::unique_ptr<ID3D11DeviceContext, std::function<void(ID3D11DeviceContext*)>> immediate_context_; ///< �R���e�L�X�g
        std::unique_ptr<IDXGISwapChain, std::function<void(IDXGISwapChain*)>> swap_chain_;                  ///< �X���b�v�`�F�[��
        D3D_DRIVER_TYPE driver_type_;                                                                       ///< �h���C�o�[���
        D3D_FEATURE_LEVEL feature_level_;                                                                   ///< �@�\���x��
      };
    };

    /**
     *  @brief  �f�o�C�X���쐬����
     *  @param  handle:�E�B���h�E�n���h��
     *  @param  width:�E�B���h�E�̉���
     *  @param  height:�E�B���h�E�̏c��
     */
    std::shared_ptr<IDevice> IDevice::Create(const HWND& handle, const std::uint32_t& width, const std::uint32_t& height)
    {
      auto device = std::make_shared<Device>();
    
      device->Create(handle, width, height);
    
      return device;
    }
  };
};