//=============================================================================
//
// �T�E���h���� [sound.h]
// Author ��������
//
//=============================================================================
#ifndef _SOUND_H_					//���̃}�N����`������Ă��Ȃ�������
#define _SOUND_H_					//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

// �N���X��`(CSound)
class CSound
{
public:			// �N�ł��A�N�Z�X�o����

	//-------------------------------------------------------------------------
	// �T�E���h�ꗗ
	//-------------------------------------------------------------------------
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g��BGM
		SOUND_LABEL_BGM_GAME,			// �Q�[��BGM
		SOUND_LABEL_BGM_RESULT,			// ���U���gBGM
		SOUND_LABEL_BGM_RANKING,		// �����L���OBGM
		SOUND_LABEL_SE_RUN,				// ���s��
		SOUND_LABEL_SE_RUNSTART,		// ����n�߉�
		SOUND_LABEL_SE_JUMP,			// �W�����v��
		SOUND_LABEL_SE_SELECT,			// �I����
		SOUND_LABEL_SE_DECIDE,			// ���艹
		SOUND_LABEL_SE_NEWRECORD,		// �V�L�^�̉�
		SOUND_LABEL_SE_HOVER,			// �z�o�[�W�F�b�g
		SOUND_LABEL_SE_LANDING,			// ���n��
		SOUND_LABEL_SE_JETDASH,			// �W�F�b�g�_�b�V��
		SOUND_LABEL_SE_STEPPING,		// ���݂���
		SOUND_LABEL_SE_ENEMYBREAK,		// �G�̔j��
		SOUND_LABEL_SE_SMASH,			// ������΂���
		SOUND_LABEL_SE_SMASHFLY,		// ������ђ��̉�
		SOUND_LABEL_SE_GETITEM,			// �A�C�e���擾��
		SOUND_LABEL_SE_CANNON,			// ��C�̉�
		SOUND_LABEL_SE_GROUNDQUAKE,		// �O���E���h�N�G�C�N��
		SOUND_LABEL_SE_STUN,			// �C�≹
		SOUND_LABEL_SE_DAMAGE,			// �_���[�W��
		SOUND_LABEL_MAX					// ���̗񋓌^�̑���
	};

	//-----------------------------------------------------------------------------
	// �T�E���h���̍\���̒�`
	//-----------------------------------------------------------------------------
	struct SOUNDINFO
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	};

	CSound();			// �R���X�g���N�^
	~CSound();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// �����o�ϐ�
	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
	static SOUNDINFO m_aSoundInfo[CSound::SOUND_LABEL_MAX];	// �T�E���h�̏��
};

#endif