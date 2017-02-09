/**
 *	@file		Direct3D12Device.cpp
 *	@brief		Direct3D12�f�o�C�X�Ɋւ��鏈�����s���v���O�����\�[�X
 *	@author		kkllPreciel
 *	@date		2017/02/07
 *	@version	1.0
 */

#if _DEBUG
#include "Debugger.h"
#include <Windows.h>
#include <sstream>
#endif
#include "Direct3D12Device.h"

namespace Sein
{
	namespace Direct3D12
	{
		/** 
		 *	@brief	�R���X�g���N�^
		 */
		Device::Device() : device(nullptr), swapChain(nullptr)
		{

		}

		/**
		 *	@brief	�f�X�g���N�^
		 */
		Device::~Device()
		{
			Release();
		}

		/**
		 *	@brief	�f�o�C�X�𐶐�����
		 */
		void Device::Create()
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

			// �R�}���h�L���[�̍쐬
			// �R�}���h�L���[��GPU�֕`�施�߂��o���ꂽ�R�}���h���X�g��
			// �������s����
			// �܂����̃A�_�v�^�[(GPU)�Ɋ֘A�t�����Ă���
			{

			}

			// �X���b�v�`�F�C���̍쐬
			// �R�}���h�L���[���w�肵�č쐬���� = �A�_�v�^�[���w�肵�č쐬����
			{

			}

			// �`��Ǘ�
			// �X���b�v�`�F�C������

			// �`��R�}���h�֘A
			// �R�}���h�L���[����
			// �R�}���h�A���P�[�^����

			// �`��ΏہE�Ǘ�
			// �t�F���X����
			// �f�B�X�N���v�^�[�q�[�v�̍쐬
			// �����_�[�^�[�Q�b�g�r���[�̍쐬
		}

		/**
		 *	@brief	�f�o�C�X���J������
		 */
		void Device::Release()
		{

		}

		/**
		 *	@brief	��ʂ��X�V����
		 */
		void Device::Present()
		{

		}
	};
};