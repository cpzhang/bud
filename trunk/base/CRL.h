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

		// 压缩(标志数据与传入的数据的字节数相同)
		// pSrc: 源数据指针
		// pDst: 目标数据指针
		// nSize:源数据size(字节数)
		// 返回值：失败->0,成功->压缩后的size(字节数)
		// 格式：...;n-1;n个不同值a1;a2;...;an;k+127;k个相同值;b;...
		BOOL Compress(T* pSrc, T* pDst, int nSize)
		{
			if (!pSrc || !pDst) return FALSE;

			const int nBytes = sizeof(T);		// 每个数据占多少字节
			int nSameDataRepeatCount = 0;		// 相同数据连续出现的次数
			int nDifferentDataRepeatCount = 0;	// 不同数据连续出现的次数
			int nRlePos = 0;					// 写入rle缓冲的位置
			BYTE* buf = (BYTE*)pSrc;
			BYTE* from = buf;
			int nBufferLength = nSize / nBytes;

			if (nBufferLength == 1) // 只有一个数据
			{
				pDst[nRlePos++] = 0;
				pDst[nRlePos++] = pSrc[0];
			}
			
			for (int i = 1; i < nBufferLength; ++i)
			{
				if (memcmp(buf, buf + nBytes, nBytes)) // 两个数据不相同
				{
					if (nSameDataRepeatCount > 0) // 写入前面连续相同的数据
					{
						pDst[nRlePos++] = MAX_REPEAT_COUNT + nSameDataRepeatCount;
						memcpy(&pDst[nRlePos++], from, nBytes);
						from = buf + nBytes;
						nSameDataRepeatCount = 0;
						nDifferentDataRepeatCount = 0;
					}
					else // 连续不同的数据数目加1
					{ 
						nDifferentDataRepeatCount ++;
					}
				}
				else // 两个数据相同
				{
					if (nDifferentDataRepeatCount > 0) // 写入前面连续不同的数据
					{
						pDst[nRlePos++] = nDifferentDataRepeatCount - 1;
						memcpy(&pDst[nRlePos], from, nBytes*nDifferentDataRepeatCount);
						nRlePos += nDifferentDataRepeatCount;
						from = buf; 
						nDifferentDataRepeatCount = 0;
						nSameDataRepeatCount = 1;
					}
					else // 连续相同的数据数目加1
					{
						nSameDataRepeatCount ++;
					}
				}
				
				// 对最大连续数目的判断
				if (nSameDataRepeatCount >= MAX_REPEAT_COUNT) // 连续相同的数据数目已经达到了最大值
				{
					pDst[nRlePos++] = MAX_REPEAT_COUNT + nSameDataRepeatCount - 1;
					memcpy(&pDst[nRlePos++], from, nBytes);
					from = buf + nBytes;
					nDifferentDataRepeatCount = 0;
					nSameDataRepeatCount = 0;
				}
				else if (nDifferentDataRepeatCount >= MAX_REPEAT_COUNT) // 连续不同的数据数目已经达到了最大值
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
			
			// 尾部处理
			if (nSameDataRepeatCount > 0) // 本行结束前有连续的相同数据
			{
				pDst[nRlePos++] = MAX_REPEAT_COUNT + nSameDataRepeatCount;
				memcpy(&pDst[nRlePos++], from, nBytes);
			}
			else if (nDifferentDataRepeatCount > 0)  // 否则，本行结束前有不同的连续数据
			{
				pDst[nRlePos++] = nDifferentDataRepeatCount;
				memcpy(&pDst[nRlePos], from, nBytes*(nDifferentDataRepeatCount + 1));
				nRlePos += nDifferentDataRepeatCount + 1;
			}
			
			return nRlePos*nBytes;
		}

		
		// 解压缩
		// pSrc:源数据指针
		// pDst:目标数据指针
		// nSize:源数据size(字节数)
		// 返回值：失败->0,成功->解压缩后的size(字节数)
		BOOL DisCompress(T* pSrc, T* pDst, int nSize)
		{
			if (!pSrc || !pDst) return FALSE;
			
			const int nBytes = sizeof(T);		// 每个数据占多少字节
			int nSameDataRepeatCount = 0;		// 相同数据连续出现的次数
			int nDifferentDataRepeatCount = 0;	// 不同数据连续出现的次数
			int nPos = 0;						// 写入缓冲的位置
			int nBufferLength = nSize / nBytes;

			for (int i=0; i<nBufferLength;)
			{
				if (pSrc[i] >= MAX_REPEAT_COUNT) // 连续相同的数据
				{
					nSameDataRepeatCount = pSrc[i++] - MAX_REPEAT_COUNT + 1;
					while (nSameDataRepeatCount-- > 0)
						pDst[nPos++] = pSrc[i];
					i++;
				}
				else // 连续不同的数据
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
