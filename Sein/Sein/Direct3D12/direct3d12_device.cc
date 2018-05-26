/**
 *  @file     direct3d12_device.cc
 *  @brief    Direct3D12�f�o�C�X�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2017/02/07
 *  @version  1.0
 */

#include "direct3d12_device.h"

#include <d3d12.h>
#include <wrl\client.h>
#include <dxgi1_4.h>

#include "DirectXTK12/include/ResourceUploadBatch.h"

#include "fence.h"
#include "texture_view.h"
#include "depth_stencil_view.h"
#include "descriptor_range.h"
#include "root_parameter.h"
#include "root_signature.h"
#include "rasterizer_desc.h"
#include "graphics_pipeline_state.h"
#include "command_queue.h"
#include "swap_chain.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      constexpr std::uint32_t FRAME_COUNT = 2;

      /**
       *  @brief  �f�o�C�X�p�N���X
       */
      class Device final : public IDevice
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        Device() : device_(nullptr, [](ID3D12Device* p) { if (p) { p->Release(); } }), command_queue_(nullptr), swap_chain_(nullptr),
          root_signature_(nullptr), pipeline_state_(nullptr), render_target_list_(),
          descriptor_heaps_(), depthStencilView(nullptr), fence(nullptr), texBuffer()
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Device() override
        {
          Release();
        }

        /**
         *  @brief  ��������
         *  @param  handle:�E�B���h�E�n���h��
         *  @param  width:����
         *  @param  height:�c��
         */
        void Create(const HWND handle, const std::uint32_t width, const std::uint32_t height)
        {
#if _DEBUG
          // �f�o�b�O���C���[��L���ɐݒ肷��
          Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
          if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
          {
            throw "�f�o�b�O���C���[�̐����Ɏ��s���܂����B";
          }
          debugInterface->EnableDebugLayer();
#endif
          // �t�@�N�g���̐���
          // �A�_�v�^�[�̗񋓂Ɏg�p
          Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

          if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
          {
            throw "DXGI�t�@�N�g���̐����Ɏ��s���܂����B";
          }

          // Direct3D12�̃f�o�C�X���쐬����
          {
#if 1
            // �f�o�C�X�𐶐�
            // �A�_�v�^�[��񋓂��쐬�����
            // ���ɂ���ă��������[�N��������������
            // �f�t�H���g�̃A�_�v�^�[���g�p���쐬����
            ID3D12Device* pDevice;
            if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice))))
            {
              throw std::exception("�f�o�C�X�̐����Ɏ��s���܂����B");
            }
            device_.reset(pDevice);
#else
            Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;

            // �A�_�v�^�[(�r�f�I�J�[�h)��񋓂���
            for (auto i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &pAdapter); ++i)
            {
              DXGI_ADAPTER_DESC1 desc;
              pAdapter->GetDesc1(&desc);

              // TODO:����
              if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
              {
                continue;
              }

              // �f�o�C�X���쐬
              if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
              {
                continue;
              }

#if _DEBUG
              std::wostringstream ostr;
              pAdapter->GetDesc1(&desc);
              ostr << "D3D12-capable hardware found:" << desc.Description << "(" << (desc.DedicatedVideoMemory >> 20) << "MB)\n";
              OutputDebugString(ostr.str().c_str());
#endif
            }

            // WARP�Ő�������
            if (nullptr == device)
            {
#if _DEBUG
              OutputDebugString(L"WARP software adapter requested.  Initializing...\n");
#endif
              if (FAILED(factory->EnumWarpAdapter(IID_PPV_ARGS(&pAdapter))))
              {
                throw "WARP�A�_�v�^�[�����݂��܂���B";
              }

              if (FAILED(D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
              {
                throw "�f�o�C�X�̐����Ɏ��s���܂����B";
              }
            }
#endif
          }

          // �R�}���h�L���[�̍쐬
          // �R�}���h�L���[�̍쐬
          // �R�}���h�L���[��GPU�֕`�施�߂��o���ꂽ�R�}���h���X�g��
          // �������s����
          // �܂����̃A�_�v�^�[(GPU)�Ɋ֘A�t�����Ă���
          {
            D3D12_COMMAND_QUEUE_DESC command_queue_desc = {};
            command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;  // �^�C���A�E�g������L���ɂ���
            command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;  // �����_�����O�֘A�̃R�}���h���X�g
            command_queue_ = this->CreateCommandQueue(command_queue_desc);
          }

          // �X���b�v�`�F�C���̍쐬
          // �R�}���h�L���[���w�肵�č쐬���� = �A�_�v�^�[���w�肵�č쐬����
          {
            DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
            swap_chain_desc.Width = width;                                  // �E�B���h�E����
            swap_chain_desc.Height = height;                                // �E�B���h�E�c��
            swap_chain_desc.BufferCount = FRAME_COUNT;                      // �o�b�t�@�̐���2��(�t�����g�o�b�t�@���܂ނ炵�����A�����T���v����������ƃo�b�N�o�b�t�@�̐��ȋC������)
            swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // ���炭�o�b�t�@�̃t�H�[�}�b�g(4 �����A32 �r�b�g�����Ȃ�����)
            swap_chain_desc.Scaling = DXGI_SCALING_NONE;                    // ��ʃT�C�Y�ƃo�b�t�@�T�C�Y���������Ȃ����̊g�k����(�g��k���͍s��Ȃ�)
            swap_chain_desc.SampleDesc.Quality = 0;                         // �}���`�T���v�����O�̕i�����x��
            swap_chain_desc.SampleDesc.Count = 1;                           // �s�N�Z���P�ʂ̃}���`�T���v�����O��
            swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // �o�b�N�o�b�t�@�̎g�p�ړI�y��CPU�A�N�Z�X�I�v�V����(�����_�[�^�[�Q�b�g�Ƃ��Ďg�p)
            swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // �X���b�v�`�F�C���̓���I�v�V����(���[�h�ؑ։\�ɐݒ�)
            swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;     // �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̃X���b�v�����w��(�o�b�N�o�b�t�@���f�B�X�v���C�ɕ\�����ꂽ��j������)
            swap_chain_ = this->CreateSwapChain(factory.Get(), handle, swap_chain_desc);
          }

          // Alt + Enter�Ńt���X�N���[�����̋@�\�𖳌��ɐݒ�
          factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER);

          // �f�B�X�N���v�^�[�q�[�v�p�������̈�̊m��
          {
            descriptor_heaps_.resize(D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES);
          }

          // �萔�o�b�t�@�r���[�A�V�F�[�_�[���\�[�X�r���[�p�f�B�X�N���v�^�[�q�[�v�𐶐�
          {
            D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
            cbvHeapDesc.NumDescriptors = 5;                                 // �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�萔�o�b�t�@�A�V�F�[�_�[���\�[�X)
            cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;      // �萔�o�b�t�@ or �V�F�[�_�[���\�[�X(�e�N�X�`��) or �����_���A�N�Z�X �̂ǂꂩ�̃q�[�v
            cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  // �V�F�[�_�[����A�N�Z�X��

            descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV] = this->CreateDescriptorHeap(cbvHeapDesc);
          }

          // �[�x�X�e���V���r���[�p�f�B�X�N���v�^�[�q�[�v�𐶐�
          {
            D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
            heapDesc.NumDescriptors = 1;                      // �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�[�x�X�e���V���r���[)
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;   // �[�x�X�e���V���r���[�̃q�[�v
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // �V�F�[�_�[����A�N�Z�X�s��
            
            descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_DSV] = this->CreateDescriptorHeap(heapDesc);
          }

          // �����_�[�^�[�Q�b�g
          {
            // �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�[�q�[�v�̍쐬
            // �f�B�X�N���v�^�[�̓o�b�t�@�̏��f�[�^(�e�N�X�`���o�b�t�@�A���_�o�b�t�@��)
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FRAME_COUNT;             // �f�B�X�N���v�^�[�q�[�v���̃f�B�X�N���v�^�[��(�t�����g�o�b�t�@�A�o�b�N�o�b�t�@)
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;    // ��ʂ̓����_�[�^�[�Q�b�g�r���[
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // �V�F�[�_�[����Q�Ƃ��Ȃ�

            descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV] = this->CreateDescriptorHeap(rtvHeapDesc);

            // �f�B�X�N���v�^�[�̓o�^
            {
              // �t���[���o�b�t�@�������o�^����
              render_target_list_.resize(FRAME_COUNT);
              for (auto i = 0; i < FRAME_COUNT; ++i)
              {
                decltype(auto) render_target = render_target_list_[i].get();
                if (FAILED(swap_chain_->GetBuffer(i, &render_target)))
                {
                  throw std::exception("�o�b�N�o�b�t�@�̎擾�Ɏ��s���܂����B");
                }
                render_target_list_[i].reset(render_target);

                const auto& descriptor = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV]->CreateDescriptor();

                // �����_�[�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�[���쐬����
                // �f�B�X�N���v�^�[�q�[�v�̗̈�ɍ쐬�����
                device_->CreateRenderTargetView(
                  render_target,        // �����_�[ �^�[�Q�b�g��\��ID3D12Resource�ւ̃|�C���^
                  nullptr,              // D3D12_RENDER_TARGET_VIEW_DESC�ւ̃|�C���^
                  descriptor.GetHandleForCPU()
                );
              }
            }
          }

          // �t�F���X�̐���
          // Directx12�ł�GPU�̕`��̏I���҂��������ōs��Ȃ�(��������ꂸ��ʂ����������Ȃ�)
          // ���̂��ߓ�������邽�߂̃I�u�W�F�N�g(�t�F���X)���쐬����
          {
            fence = std::make_unique<Fence>();
            fence->Create(device_.get());

            // �`�揈�����s���Ă���\��������̂ŕ`��I���҂����s��
            WaitForGpu();
          }

          // �A�Z�b�g��ǂݍ���
          {
            LoadAssets(width, height);
          }
        }

        /**
         *  @brief  ���\�[�X���J������
         */
        void Release() noexcept override
        {
          // GPU�̕`��I���҂����s��
          WaitForGpu();

          for (auto& render_target : render_target_list_)
          {
            render_target->Release();
            render_target.release();
          }
        }

        /**
         *  @brief  �V�[�����J�n����
         */
        void BeginScene(ICommandList* const command_list, std::uint32_t buffer_index) override
        {
          decltype(auto) render_target = render_target_list_[buffer_index];
          decltype(auto) descriptor_heap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_RTV];
          const auto& descriptor_for_rtv = descriptor_heap->GetDescriptor(buffer_index);

          // TODO:const_cast�̍폜
          decltype(auto) graphics_command_list = const_cast<ID3D12GraphicsCommandList&>(command_list->Get());

          command_list->Begin();

          // �o�b�N�o�b�t�@���`��^�[�Q�b�g�Ƃ��Ďg�p�ł���悤�ɂȂ�܂ő҂�
          // ���\�[�X�͕`��^�[�Q�b�g, �J�ڑO��Present, �J�ڌ�͕`��^�[�Q�b�g
          command_list->ResourceBarrier(render_target.get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

          // �o�b�N�o�b�t�@��`��^�[�Q�b�g�Ƃ��Đݒ肷��
          // �f�o�C�X�֐[�x�X�e���V���r���[���o�C���h����

          auto& dsvDescriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
          const auto& dsvDescriptor = dsvDescriptorHeap->GetDescriptor(0); // TODO:�}�W�b�N�i���o�[������
          graphics_command_list.OMSetRenderTargets(1, &descriptor_for_rtv.GetHandleForCPU(), false, &dsvDescriptor.GetHandleForCPU());

          // �o�b�N�o�b�t�@���N���A����
          const float Color[] = { 0.0f, 0.0f, 0.6f, 1.0f };
          graphics_command_list.ClearRenderTargetView(descriptor_for_rtv.GetHandleForCPU(), Color, 0, nullptr);

          // �[�x�X�e���V���r���[���N���A����(�[�x�o�b�t�@�̂�)
          graphics_command_list.ClearDepthStencilView(dsvDescriptor.GetHandleForCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
        }
        
        /**
         *  @brief  �`�悷��
         *  @param  command_list:�R�}���h���X�g
         *  @param  indexCount:���_�C���f�b�N�X��
         *  @param  instanceCount:�C���X�^���X��
         */
        void Render(ICommandList* const command_list, const unsigned int indexCount, const unsigned int instanceCount) override
        {
          // TODO:const_cast�̍폜
          decltype(auto) graphics_command_list = const_cast<ID3D12GraphicsCommandList&>(command_list->Get());

          // �r���[�|�[�g�̍쐬
          D3D12_VIEWPORT viewport;
          viewport.TopLeftX = 0;
          viewport.TopLeftY = 0;
          viewport.Width = 600;
          viewport.Height = 400;
          viewport.MinDepth = 0;
          viewport.MaxDepth = 1;

          // �r���[�|�[�g�̐ݒ�
          command_list->SetViewports(1, &viewport);

          // �V�U�[��`(�V�U�[�e�X�g)�̍쐬
          D3D12_RECT scissor;
          scissor.left = 0;
          scissor.top = 0;
          scissor.right = 600;
          scissor.bottom = 400;

          // �V�U�[��`(�V�U�[�e�X�g)�̐ݒ�
          command_list->SetScissorRects(1, &scissor);

          // �p�C�v���C���X�e�[�g�̐ݒ�(�؂�ւ��Ȃ��ꍇ�́A�R�}���h���X�g���Z�b�g���ɐݒ�\)
          pipeline_state_->SetPipelineState(&graphics_command_list);

          // �O���t�B�b�N�X�p�C�v���C���̃��[�g�V�O�l�`����ݒ肷��
          root_signature_->SetGraphicsRootSignature(&graphics_command_list);

          // �`��Ɏg�p����f�B�X�N���v�^�[�q�[�v��ݒ�
          auto& descriptor_heap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
          descriptor_heap->SetDescriptorHeaps(&graphics_command_list);

          // �f�B�X�N���v�[�^�q�[�v�e�[�u����ݒ�
          // TODO:�f�B�X�N���v�^�[�e�[�u���̐ݒ���f�B�X�N���v�^�[�q�[�v�Ɉړ�����
          graphics_command_list.SetGraphicsRootDescriptorTable(0, descriptor_heap->GetDescriptor(0).GetHandleForGPU());  // �萔�o�b�t�@�p�f�B�X�N���v�[�^�q�[�v�e�[�u��
          graphics_command_list.SetGraphicsRootDescriptorTable(1, descriptor_heap->GetDescriptor(1).GetHandleForGPU());  // StructuredBuffer�p�f�B�X�N���v�[�^�q�[�v�e�[�u��
          //graphics_command_list.SetGraphicsRootDescriptorTable(2, descriptor_heap->GetDescriptor(2).GetHandleForGPU());  // �e�N�X�`���p�f�B�X�N���v�[�^�q�[�v�e�[�u��

          // �`��R�}���h�̐���
          graphics_command_list.DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
        }
        
        /**
         *  @brief  �V�[�����I������
         */
        void EndScene(ICommandList* const command_list, std::uint32_t buffer_index) override
        {
          decltype(auto) render_target = render_target_list_[buffer_index];

          // �o�b�N�o�b�t�@�̕`�抮����҂��߂̃o���A��ݒu
          // ���\�[�X�͕`��^�[�Q�b�g, �J�ڑO�͕`��^�[�Q�b�g, �J�ڌ��Present
          command_list->ResourceBarrier(render_target.get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

          // �R�}���h���X�g���N���[�Y����
          command_list->End();
        }
        
        /**
         *  @brief  ��ʂ��X�V����
         */
        void Present() override
        {
          // �`��I���҂����s��
          WaitForGpu();

          // ��ʂ̍X�V
          swap_chain_->Present(1, 0);
        }
        
        /**
         *  @brief  �萔�o�b�t�@���쐬����
         *  @param  size_in_bytes:�萔�o�b�t�@�̃T�C�Y
         *  @return �萔�o�b�t�@�̃��j�[�N�|�C���^
         */
        std::unique_ptr<IConstantBuffer> CreateConstantBuffer(const std::uint32_t size_in_bytes) override
        {
          auto& descriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
          return this->CreateConstantBuffer(descriptorHeap, size_in_bytes);
        }

        /**
         *  @brief  �萔�o�b�t�@���쐬����
         *  @param  descriptor_heap:�萔�o�b�t�@���쐬����f�B�X�N���v�^�[�q�[�v
         *  @param  size_in_bytes:�萔�o�b�t�@�̃T�C�Y
         *  @return �萔�o�b�t�@�̃��j�[�N�|�C���^
         */
        std::unique_ptr<IConstantBuffer> CreateConstantBuffer(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t size_in_bytes) override
        {
          auto& descriptor = descriptor_heap->CreateDescriptor();
          return IConstantBuffer::Create(device_.get(), descriptor.GetHandleForCPU(), size_in_bytes);
        }
        
        /**
         *  @brief  �V�F�[�_�[���\�[�X�o�b�t�@���쐬����
         *  @param  num:���\�[�X���̗v�f��
         *  @param  size:���\�[�X����1�v�f�̃T�C�Y
         *  @return �V�F�[�_�[���\�[�X�o�b�t�@�̃��j�[�N�|�C���^
         */
        std::unique_ptr<ShaderResourceBuffer> CreateShaderResourceBuffer(const unsigned int num, const unsigned int size) override
        {
          // �q�[�v�̐ݒ�
          D3D12_HEAP_PROPERTIES properties;
          properties.Type = D3D12_HEAP_TYPE_UPLOAD;                     // �q�[�v�̎��(�����CPU�AGPU����A�N�Z�X�\�ȃq�[�v�ɐݒ�)
          properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN; // CPU�y�[�W�v���p�e�B(�s���ɐݒ�)
          properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;  // �q�[�v�̃������v�[��(�s���ɐݒ�)
          properties.CreationNodeMask = 1;                              // ���炭�q�[�v�����������A�_�v�^�[(GPU)�̔ԍ�
          properties.VisibleNodeMask = 1;                               // ���炭�q�[�v���\�������A�_�v�^�[(GPU)�̔ԍ�

          auto shaderResourceBuffer = std::make_unique<ShaderResourceBuffer>();
          auto& descriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
          auto& descriptor = descriptorHeap->CreateDescriptor();
          shaderResourceBuffer->Create(device_.get(), descriptor.GetHandleForCPU(), num, size);

          return shaderResourceBuffer;
        }
        
        /**
         *  @brief  �R�}���h���X�g���쐬����
         *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
         *  @return �R�}���h���X�g�̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<ICommandList> CreateCommandList(const D3D12_COMMAND_LIST_TYPE& command_list_type) override
        {
          return ICommandList::Create(device_.get(), command_list_type);
        }
        
        /**
         *  @brief  ���_�o�b�t�@���쐬����
         *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
         *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
         */
        std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) override
        {
          return IVertexBuffer::Create(device_.get(), size_in_bytes);
        }
        
        /**
         *  @brief  ���_�C���f�b�N�X�o�b�t�@���쐬����
         *  @param  size_in_bytes:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(���_�C���f�b�N�X�T�C�Y * ���_�C���f�b�N�X��)
         *  @return ���_�C���f�b�N�X�o�b�t�@�ւ̃��j�[�N�|�C���^
         */
        std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) override
        {
          return IIndexBuffer::Create(device_.get(), size_in_bytes);
        }
        
        /**
         *  @brief  �V�F�[�_�[���쐬����
         *  @param  shader_file_path:�V�F�[�_�[�t�@�C���̃p�X
         *  @return �V�F�[�_�[�ւ̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<IShader> CreateShader(const std::string& shader_file_path) override
        {
          return IShader::Create(shader_file_path);
        }
        
        /**
         *  @brief  ���Ƀo�b�N�o�b�t�@�ɂȂ�o�b�t�@�̔ԍ����擾����
         *  @return ���Ƀo�b�N�o�b�t�@�ɂȂ�o�b�t�@�̔ԍ�
         */
        const std::uint32_t GetNextBackBufferIndex() const noexcept override
        {
          return (swap_chain_->GetCurrentBackBufferIndex() + 1) % render_target_list_.size();
        }
        
        /**
         *  @brief  �t�@�C������e�N�X�`���o�b�t�@�𐶐�����
         *  @param  file_path:�e�N�X�`���t�@�C���p�X
         */
        void CreateTextureBufferFromFile(const std::wstring& file_path) override
        {
          DirectX::ResourceUploadBatch upload_batch(device_.get());
          upload_batch.Begin();

          decltype(auto) texture = ITexture::CreateFromFile(device_.get(), upload_batch, file_path);

          // TODO:const_cast���폜����
          auto uploadResourcesFinished = upload_batch.End(const_cast<ID3D12CommandQueue*>(&(command_queue_->Get())));

          WaitForGpu();

          uploadResourcesFinished.wait();

          texBuffer.emplace_back(std::make_unique<TextureView>());
          auto texture_view = texBuffer[texBuffer.size() - 1].get();

          texture_view->Create(device_.get(), texture.release(), descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].get());
        }
        
        /**
         *  @brief  �R�}���h���X�g�����s����
         *  @param  command_list:�R�}���h���X�g
         */
        void ExecuteCommandLists(ICommandList* const command_list) override
        {
          // TODO:const_cast�̍폜
          decltype(auto) graphics_command_list = const_cast<ID3D12GraphicsCommandList&>(command_list->Get());

          ID3D12CommandList* ppCommandLists[] = { &graphics_command_list };
          command_queue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
        }

        /**
         *  @brief  �f�B�X�N���v�^�[�q�[�v�𐶐�����
         *  @param  descriptor_heap_desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ�
         *  @return �f�B�X�N���v�^�[�q�[�v�̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<IDescriptorHeap> CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& descriptor_heap_desc) override
        {
          return IDescriptorHeap::Create(device_.get(), descriptor_heap_desc);
        }

      private:
        /**
         *  @brief  �A�Z�b�g��ǂݍ���
         *  @param  width:�E�B���h�E����
         *  @param  height:�E�B���h�E�c��
         */
        void LoadAssets(const std::uint32_t width, const std::uint32_t height)
        {
          // �[�x�X�e���V���r���[�̍쐬
          {
            depthStencilView = std::make_unique<DepthStencilView>();
            auto& descriptorHeap = descriptor_heaps_[D3D12_DESCRIPTOR_HEAP_TYPE_DSV];
            auto& descriptor = descriptorHeap->CreateDescriptor();
            depthStencilView->Create(device_.get(), descriptor.GetHandleForCPU(), width, height);
          }

          // ���[�g�V�O�l�`���̍쐬
          {
            // �萔�o�b�t�@�̃f�B�X�N���v�^�[�����W���쐬
            auto descriptor_range_for_cbv = IDescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1);

            // StructuredBuffer(�V�F�[�_�[���\�[�X)�̃f�B�X�N���v�^�[�����W���쐬
            auto descriptor_range_for_sbv = IDescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1);

            // �e�N�X�`��(�V�F�[�_�[���\�[�X)�̃f�B�X�N���v�^�[�����W���쐬
            auto descriptor_range_for_texture = IDescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3);

            // ���[�g�p�����[�^�̐ݒ�
            D3D12_ROOT_PARAMETER rootParameters[3];

            // �萔�o�b�t�@�p���[�g�p�����[�^
            auto root_parameter_for_cbv = IRootParameter::CreateForDescriptorTable(*descriptor_range_for_cbv, D3D12_SHADER_VISIBILITY_VERTEX);
            rootParameters[0] = root_parameter_for_cbv->Get();

            // StructuredBuffer�p���[�g�p�����[�^
            auto root_parameter_for_sbv = IRootParameter::CreateForDescriptorTable(*descriptor_range_for_sbv, D3D12_SHADER_VISIBILITY_VERTEX);
            rootParameters[1] = root_parameter_for_sbv->Get();

            // �e�N�X�`���p���[�g�p�����[�^
            auto root_paramter_for_texture = IRootParameter::CreateForDescriptorTable(*descriptor_range_for_texture, D3D12_SHADER_VISIBILITY_PIXEL);
            rootParameters[2] = root_paramter_for_texture->Get();


            // �T���v���[�̐ݒ�
            D3D12_STATIC_SAMPLER_DESC sampler = {};
            sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;					// �t�B���^�����O���@(�|�C���g�T���v�����O)
            sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// �͈͊O�ɂ���U���W�̉������@(���E���̐F���g�p)
            sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// �͈͊O�ɂ���V���W�̉������@(���E���̐F���g�p)
            sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;				// �͈͊O�ɂ���W���W�̉������@(���E���̐F���g�p)
            sampler.MipLODBias = 0;												// �~�b�v�}�b�v ���x������̃I�t�Z�b�g
            sampler.MaxAnisotropy = 0;											// D3D12_FILTER_ANISOTROPIC��D3D12_FILTER_COMPARISON_ANISOTROPIC���t�B���^�Ƃ��Ďw�肵���ꍇ�̃N�����v�l
            sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;				// �����̃T���v�����O�f�[�^�ɑ΂��ăf�[�^���r����֐�(��r���Ȃ�)
            sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �͈͊O���W�̉������@�ɑ΂���D3D12_TEXTURE_ADDRESS_MODE_BORDER���w�肳��Ă���ꍇ�Ɏg�p����鋫�E�̐F
            sampler.MinLOD = 0.0f;												// �N�����v����~�b�v�}�b�v�͈͂̉���
            sampler.MaxLOD = D3D12_FLOAT32_MAX;									// �N�����v����~�b�v�}�b�v�͈͂̏��(������Ȃ��ꍇ��D3D12_FLOAT32_MAX���̑傫���T�C�Y���w�肷��)
            sampler.ShaderRegister = 0;											// ���W�X�^�[�ԍ�(�uregister(s2, space3)�v��s2�̔ԍ��w��p)
            sampler.RegisterSpace = 0;											// ���W�X�^�[�X�y�[�X(�uregister(s2, space3)�v��space3�̔ԍ��w��p)
            sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_�[����A�N�Z�X�\

                                                                          // ���[�g�V�O�l�`���̐ݒ�
            D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
            rootSignatureDesc.NumParameters = 3;													// ���[�g�V�O�l�`���̃X���b�g��
            rootSignatureDesc.pParameters = rootParameters;											// �X���b�g�̍\��?
            rootSignatureDesc.NumStaticSamplers = 1;												// �ÓI�T���v���[��
            rootSignatureDesc.pStaticSamplers = &sampler;											// �ÓI�T���v���[�ݒ�f�[�^�̃|�C���^
            rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT	// �I�v�V����(�`��Ɏg�p����)
              | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS							// �n���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
              | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS							// �h���C���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~
              | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;						// �W�I���g���V�F�[�_���烋�[�g�V�O�l�`���ւ̃A�N�Z�X�֎~

                                                                                      // ���[�g�V�O�l�`���̍쐬
            root_signature_ = IRootSignature::Create(device_.get(), rootSignatureDesc);
          }

          // �p�C�v���C���X�e�[�g�̍쐬
          {
            // �V�F�[�_�[�t�@�C���̓ǂݍ���
            // TODO:���΃p�X���w��ł���悤��
            auto vertex_shader = this->CreateShader("D:/DiskD/Study/Multithread/LearnMultithreadedRendering/LearnMultithreadedRendering/x64/Debug/vertex.cso");
            auto pixel_shader = this->CreateShader("D:/DiskD/Study/Multithread/LearnMultithreadedRendering/LearnMultithreadedRendering/x64/Debug/pixel.cso");

            // ���_���̓��C�A�E�g
            D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
            {
              // ���_�f�[�^
              { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            };

            // �f�t�H���g�ݒ�̃��X�^���C�U�[�X�e�[�g�̐ݒ�
            auto rasterizer_desc = CreateDefaultRasterizerDesc();

            // �����_�[�^�[�Q�b�g�̃u�����h��Ԃ̐ݒ�
            const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTarggetBlendDesc =
            {
              false,                        // �u�����f�B���O��L���ɂ��邩(����͖���)
              false,                        // �_�����Z��L���ɂ��邩(����͖���)
              D3D12_BLEND_ONE,              // RGB�l�s�N�Z���V�F�[�_�o�͂ɑ΂��Ď��s���鑀����w��
              D3D12_BLEND_ZERO,             // �����_�[�^�[�Q�b�g�̌��݂�RGB�l�ɑ΂��Ď��s���铮����w�肷��
              D3D12_BLEND_OP_ADD,           // RGB�܂��̓A���t�@�u�����f�B���O������w��(�\�[�X1�ƃ\�[�X2��ǉ�)
              D3D12_BLEND_ONE,              // �s�N�Z���V�F�[�_�o�͂���A���t�@�l�ɑ΂��Ď��s���铮����w��
              D3D12_BLEND_ZERO,             // �����_�[�^�[�Q�b�g�Ō��݂̃A���t�@�l�ɑ΂��Ď��s���铮��
              D3D12_BLEND_OP_ADD,           // RBG�܂��̓A���t�@�u�����f�B���O������w��(�\�[�X1�ƃ\�[�X2��ǉ�)
              D3D12_LOGIC_OP_NOOP,          // �����_�[�^�[�Q�b�g�ɐݒ肷��_�����Z?(�m�[�I�y���[�V�����A�������Ȃ�)
              D3D12_COLOR_WRITE_ENABLE_ALL  // 
            };

            // �u�����h��Ԃ̐ݒ�
            D3D12_BLEND_DESC blendDesc;
            blendDesc.AlphaToCoverageEnable = false;  // �A���t�@�g�D�J�o���b�W��L���ɂ��邩
            blendDesc.IndependentBlendEnable = false; // ���������_�[�^�[�Q�b�g�ɓƗ������u�����h��L���ɂ��邩�ǂ������w�肵�܂�
            for (unsigned int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
              blendDesc.RenderTarget[i] = defaultRenderTarggetBlendDesc;

            // �[�x�X�e���V����Ԃ̐ݒ�
            D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
            depthStencilDesc.DepthEnable = true;                          // �f�v�X�e�X�g��L���ɂ��邩?
            depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �[�x�X�e���V�� �o�b�t�@�[�ւ̏������݂��I���ɂ��܂�
            depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;      // �[�x�f�[�^�������̐[�x�f�[�^�Ɣ�r����֐��ł��B�\�[�X�f�[�^���Ώۃf�[�^�����������ꍇ�A��r�ɍ��i���܂��B
            depthStencilDesc.StencilEnable = false;                       // �X�e���V���e�X�g��L���ɂ��邩?

                                                                          // �}���`�T���v�����O�p�����[�^�[�̐ݒ�
            DXGI_SAMPLE_DESC sampleDesc = {};
            sampleDesc.Count = 1; // �s�N�Z���P�ʂ̃}���`�T���v�����O�̐�

                                  // �p�C�v���C���X�e�[�g�̐ݒ�
            D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc = {};
            pipeline_state_desc.InputLayout = { inputElementDescs, _countof(inputElementDescs) }; // ���̓��C�A�E�g�̍\��
            pipeline_state_desc.RasterizerState = rasterizer_desc;                                // ���X�^���C�U�̏��
            pipeline_state_desc.VS = vertex_shader->Get();                                        // ���_�V�F�[�_�[�̍\��
            pipeline_state_desc.PS = pixel_shader->Get();                                         // �s�N�Z���V�F�[�_�[�̍\��
            pipeline_state_desc.BlendState = blendDesc;                                           // �u�����h��Ԃ̍\��
            pipeline_state_desc.DepthStencilState = depthStencilDesc;                             // �[�x�X�e���V����Ԃ̍\��
            pipeline_state_desc.SampleMask = UINT_MAX;                                            // �u�����h�̏�Ԃ̂��߂̃T���v���̃}�X�N
            pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   // ���̓v���~�e�B�u(�O�p�`)
            pipeline_state_desc.NumRenderTargets = 1;                                             // �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g��
            pipeline_state_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;                       // �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g
            pipeline_state_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                // �[�x�X�e���V���̃t�H�[�}�b�g
            pipeline_state_desc.SampleDesc = sampleDesc;                                          // �T���v�����O��Ԃ̍\��

                                                                                                  // ���[�g�V�O�l�`�����p�C�v���C���X�e�[�g�ɐݒ肷��
            root_signature_->SetGraphicsPipelineStateDesc(&pipeline_state_desc);

            // �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
            pipeline_state_ = IGraphicsPipelineState::Create(device_.get(), pipeline_state_desc);
          }
        }

        /**
         *  @brief  �R�}���h�L���[���쐬����
         *  @param  command_queue_desc:�R�}���h�L���[�̐ݒ�
         *  @return �R�}���h�L���[�ւ̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<ICommandQueue> Device::CreateCommandQueue(const D3D12_COMMAND_QUEUE_DESC& command_queue_desc)
        {
          // �R�}���h�L���[�̍쐬
          // �R�}���h�L���[��GPU�֕`�施�߂��o���ꂽ�R�}���h���X�g��
          // �������s����
          // �܂����̃A�_�v�^�[(GPU)�Ɋ֘A�t�����Ă���
          return ICommandQueue::Create(device_.get(), command_queue_desc);
        }

        /**
         *  @brief  �X���b�v�`�F�[�����쐬����
         *  @param  factory:DXGI�̃t�@�N�g��
         *  @param  command_queue:�R�}���h�L���[
         *  @param  handle:�E�B���h�E�n���h��
         *  @param  swap_chain_desc:�X���b�v�`�F�[���̐ݒ�
         *  @return �X���b�v�`�F�[���ւ̃V�F�A�[�h�|�C���^
         */
        std::shared_ptr<ISwapChain> Device::CreateSwapChain(IDXGIFactory4* const factory, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc)
        {
          // TODO:const_cast���폜����
          return ISwapChain::Create(factory, const_cast<ID3D12CommandQueue*>(&(command_queue_->Get())), handle, swap_chain_desc);
        }

        /**
         *  @brief  GPU�̏����҂����s��
         */
        void WaitForGpu()
        {
          // TODO:const_cast���폜����
          fence->Wait(const_cast<ID3D12CommandQueue*>(&(command_queue_->Get())));
        }

      private:
        std::unique_ptr<ID3D12Device, std::function<void(ID3D12Device*)>> device_;          ///< �f�o�C�X
        std::shared_ptr<ISwapChain> swap_chain_;        ///< �X���b�v�`�F�[��
        std::shared_ptr<ICommandQueue> command_queue_;  ///< �R�}���h�L���[


        // TODO:�����艺�͕ʃN���X�Ɉړ�����
        std::shared_ptr<IRootSignature> root_signature_;                  ///< ���[�g�V�O�l�`��
        std::shared_ptr<IGraphicsPipelineState> pipeline_state_;          ///< �p�C�v���C���X�e�[�g
        std::vector<std::shared_ptr<IDescriptorHeap>> descriptor_heaps_;  ///< �f�B�X�N���v�^�[�q�[�v�z��
        std::unique_ptr<Fence> fence;                                     ///< �t�F���X�ړ�
        std::unique_ptr<DepthStencilView> depthStencilView;               ///< �[�x�X�e���V���r���[
        std::vector<std::unique_ptr<TextureView>> texBuffer;              ///<�e�N�X�`���o�b�t�@
        std::vector<std::unique_ptr<ID3D12Resource>> render_target_list_; ///< �����_�[�^�[�Q�b�g�̃��X�g
      };
    };

    /**
     *  @brief  �f�o�C�X�𐶐�����
     *  @param  handle:�E�B���h�E�n���h��
     *  @param  width:����
     *  @param  height:�c��
     *  @return �f�o�C�X�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IDevice> IDevice::Create(const HWND handle, const std::uint32_t width, const std::uint32_t height)
    {
      auto device = std::make_shared<Device>();

      device->Create(handle, width, height);

      return device;
    }
  };
};