/********************************************************************
**	file: 		CRL.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-1-25
**	purpose:	
*********************************************************************/
#ifndef __CRL_h__
#define __CRL_h__

namespace Zen
{
	template <class T = BYTE>
	class /*_BUHHDA_EXPORT_*/ CRL
	{
	protected:
		enum {MAX_REPEAT_COUNT=0x80};
	public:
		CRL()  {}
		~CRL() {}

		// ѹ��(��־�����봫������ݵ��ֽ�����ͬ)
		// pSrc: Դ����ָ��
		// pDst: Ŀ������ָ��
		// nSize:Դ����size(�ֽ���)
		// ����ֵ��ʧ��->0,�ɹ�->ѹ�����size(�ֽ���)
		// ��ʽ��...;n-1;n����ֵͬa1;a2;...;an;k+127;k����ֵͬ;b;...
		BOOL Compress(T* pSrc, T* pDst, int nSize)
		{
			if (!pSrc || !pDst) return FALSE;

			const int nBytes = sizeof(T);		// ÿ������ռ�����ֽ�
			int nSameDataRepeatCount = 0;		// ��ͬ�����������ֵĴ���
			int nDifferentDataRepeatCount = 0;	// ��ͬ�����������ֵĴ���
			int nRlePos = 0;					// д��rle�����λ��
			BYTE* buf = (BYTE*)pSrc;
			BYTE* from = buf;
			int nBufferLength = nSize / nBytes;

			if (nBufferLength == 1) // ֻ��һ������
			{
				pDst[nRlePos++] = 0;
				pDst[nRlePos++] = pSrc[0];
			}
			
			for (int i = 1; i < nBufferLength; ++i)
			{
				if (memcmp(buf, buf + nBytes, nBytes)) // �������ݲ���ͬ
				{
					if (nSameDataRepeatCount > 0) // д��ǰ��������ͬ������
					{
						pDst[nRlePos++] = MAX_REPEAT_COUNT + nSameDataRepeatCount;
						memcpy(&pDst[nRlePos++], from, nBytes);
						from = buf + nBytes;
						nSameDataRepeatCount = 0;
						nDifferentDataRepeatCount = 0;
					}
					else // ������ͬ��������Ŀ��1
					{ 
						nDifferentDataRepeatCount ++;
					}
				}
				else // ����������ͬ
				{
					if (nDifferentDataRepeatCount > 0) // д��ǰ��������ͬ������
					{
						pDst[nRlePos++] = nDifferentDataRepeatCount - 1;
						memcpy(&pDst[nRlePos], from, nBytes*nDifferentDataRepeatCount);
						nRlePos += nDifferentDataRepeatCount;
						from = buf; 
						nDifferentDataRepeatCount = 0;
						nSameDataRepeatCount = 1;
					}
					else // ������ͬ��������Ŀ��1
					{
						nSameDataRepeatCount ++;
					}
				}
				
				// �����������Ŀ���ж�
				if (nSameDataRepeatCount >= MAX_REPEAT_COUNT) // ������ͬ��������Ŀ�Ѿ��ﵽ�����ֵ
				{
					pDst[nRlePos++] = MAX_REPEAT_COUNT + nSameDataRepeatCount - 1;
					memcpy(&pDst[nRlePos++], from, nBytes);
					from = buf + nBytes;
					nDifferentDataRepeatCount = 0;
					nSameDataRepeatCount = 0;
				}
				else if (nDifferentDataRepeatCount >= MAX_REPEAT_COUNT) // ������ͬ��������Ŀ�Ѿ��ﵽ�����ֵ
				{
					pDst[nRlePos++] = nDifferentDataRepeatCount - 1;
					memcpy(&pDst[nRlePos], from, nBytes*nDifferentDataRepeatCount);
					nRlePos += nDifferentDataRepeatCount;
					from = buf + nBytes;
					nDifferentDataRepeatCount = 0;
					nSameDataRepeatCount = 0;
				}
				
				buf += nBytes;
			}
			
			// β������
			if (nSameDataRepeatCount > 0) // ���н���ǰ����������ͬ����
			{
				pDst[nRlePos++] = MAX_REPEAT_COUNT + nSameDataRepeatCount;
				memcpy(&pDst[nRlePos++], from, nBytes);
			}
			else if (nDifferentDataRepeatCount > 0)  // ���򣬱��н���ǰ�в�ͬ����������
			{
				pDst[nRlePos++] = nDifferentDataRepeatCount;
				memcpy(&pDst[nRlePos], from, nBytes*(nDifferentDataRepeatCount + 1));
				nRlePos += nDifferentDataRepeatCount + 1;
			}
			
			return nRlePos*nBytes;
		}

		
		// ��ѹ��
		// pSrc:Դ����ָ��
		// pDst:Ŀ������ָ��
		// nSize:Դ����size(�ֽ���)
		// ����ֵ��ʧ��->0,�ɹ�->��ѹ�����size(�ֽ���)
		BOOL DisCompress(T* pSrc, T* pDst, int nSize)
		{
			if (!pSrc || !pDst) return FALSE;
			
			const int nBytes = sizeof(T);		// ÿ������ռ�����ֽ�
			int nSameDataRepeatCount = 0;		// ��ͬ�����������ֵĴ���
			int nDifferentDataRepeatCount = 0;	// ��ͬ�����������ֵĴ���
			int nPos = 0;						// д�뻺���λ��
			int nBufferLength = nSize / nBytes;

			for (int i=0; i<nBufferLength;)
			{
				if (pSrc[i] >= MAX_REPEAT_COUNT) // ������ͬ������
				{
					nSameDataRepeatCount = pSrc[i++] - MAX_REPEAT_COUNT + 1;
					while (nSameDataRepeatCount-- > 0)
						pDst[nPos++] = pSrc[i];
					i++;
				}
				else // ������ͬ������
				{
					nDifferentDataRepeatCount = pSrc[i++] + 1;
					memcpy(&pDst[nPos], &pSrc[i], nBytes*nDifferentDataRepeatCount);
					i += nDifferentDataRepeatCount;
					nPos += nBytes*nDifferentDataRepeatCount;
				}
			}
			return nPos;
		}
	};
}

#endif // __CRL_h__
