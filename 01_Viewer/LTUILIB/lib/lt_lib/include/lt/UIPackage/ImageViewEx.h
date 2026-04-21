#pragma once

#include "../lt_type.h"
#include "BCMenu.h"

#include <vector>
#include <list>
#include <algorithm>

namespace lt
{
	// CImageViewEx
	class CImageViewEx;

	//**For Label Object///////////////////////////////////////////////////////////////////////////////////
	enum class eLabelType
	{
		Text
	};

	enum class eLabelPenThick
	{
		Thin, Normal, Heavy
	};

	enum class eLabelPenStyle
	{
		Solid, Dash, Dot, DashDot, DashDotDot
	};

	enum class eLabelModifyStyle
	{
		Fix,
	};

	enum class eLabelTextAlign
	{
		LEFT_BOTTOM, CENTER_BOTTOM, RIGHT_BOTTOM,
		LEFT, CENTER, RIGHT,
		LEFT_TOP, CENTER_TOP, RIGHT_TOP
	};

	enum class eHatchStyle
	{
		NONE = -1,
		HORIZONTAL = 0,
		VERTICAL = 1,
		FDIAGONAL = 2,
		BDIAGONAL = 3,
		CROSS = 4,
		DIAGCROSS = 5,
	};

	//**Marker Object***********************************************************************************/
	struct LabelObj
	{
		virtual LPCTSTR				GetObjName() = 0;
		virtual eLabelType			GetLabelType() = 0;
		virtual RECT				GetAreaRect() = 0;

		virtual size_t				GetPointArraySize() = 0;
		virtual POINT				GetPointArray(int nIdx) = 0;
		virtual void				SetPointArray(int nIdx, POINT pt) = 0;

		virtual void				SetLabelColor(COLORREF crColor) = 0;
		virtual COLORREF			GetLabelColor() = 0;

		virtual void				SetLabelFillColor(BOOL bUse, COLORREF crColor = 0) = 0;
		virtual COLORREF			GetLabelFillColor() = 0;
		virtual BOOL				GetUseLabelFillColor() = 0;

		virtual void				SetLabelPenStyle(eLabelPenStyle style) = 0;
		virtual eLabelPenStyle		GetLabelPenStyle() = 0;

		virtual void				SetLabelPenThick(eLabelPenThick thick) = 0;
		virtual eLabelPenThick		GetLabelPenThick() = 0;

		virtual void				SetModfyStyle(eLabelModifyStyle style) = 0;
		virtual eLabelModifyStyle	GetModfyStyle() = 0;
		LabelObj() {};
		virtual ~LabelObj() {};
	};
	//****************************************************************************************************/
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	enum class eMarkerType
	{
		Rectangle, Cross, Line, Ellipse, Polygon, PolyLine, Text, CrossD
	};

	enum class eMarkerPenThick
	{
		Thin, Normal, Heavy
	};

	enum class eMarkerPenStyle
	{
		Solid, Dash, Dot, DashDot, DashDotDot
	};

	enum eMarkerModifyStyle
	{
		Fix, Move, Modify,
	};

	enum class eMarkerTextAlign
	{
		LEFT_BOTTOM, CENTER_BOTTOM, RIGHT_BOTTOM,
		LEFT, CENTER, RIGHT,
		LEFT_TOP, CENTER_TOP, RIGHT_TOP
	};

	enum class eMarkerCapStyle
	{
		None,
		ArrowLine,
		ArrowTriangle,
	};

	//**Marker Object***********************************************************************************/
	struct MarkerObj
	{
		typedef struct tagPOINTD
		{
			double x;
			double y;
		}POINTD, *LPPOINTD;

		virtual LPCTSTR				GetObjName() = 0;
		virtual eMarkerType			GetMarkerType() = 0;
		virtual RECT				GetAreaRect() = 0;

		virtual size_t				GetPointArraySize() = 0;
		virtual POINT				GetPointArray(int nIdx) = 0;
		virtual void				SetPointArray(int nIdx, POINT pt) = 0;

		//Use Only CrossD Marker (Temporary)
		virtual MarkerObj::POINTD GetPointArrayD(int nIdx) = 0;
		virtual void SetPointArrayD(int nIdx, MarkerObj::POINTD pt) = 0;

		//Use Polyline Marker Only
		virtual void				AddPoint(POINT pt) {};
		virtual void				RemovePoint(int nIdx) {};

		virtual size_t				GetMiddlePointArraySize() = 0;
		virtual POINT				GetMiddlePointArray(int nIdx) = 0;
		virtual void				GetMiddlePointArray(int nIdx, double& dx, double& dy) = 0;
		virtual void				SetMiddlePointArray(int nIdx, POINT pt) = 0;

		virtual void				SetMarkerColor(COLORREF crColor) = 0;
		virtual COLORREF			GetMarkerColor() = 0;

		virtual void				SetMarkerFillColor(BOOL bUse, COLORREF crColor = 0) = 0;
		virtual COLORREF			GetMarkerFillColor() = 0;
		virtual BOOL				GetUseMarkerFillColor() = 0;

		virtual void				SetMarkerPenStyle(eMarkerPenStyle style) = 0;
		virtual eMarkerPenStyle		GetMarkerPenStyle() = 0;

		virtual void				SetMarkerPenThick(eMarkerPenThick thick) = 0;
		virtual eMarkerPenThick		GetMarkerPenThick() = 0;

		virtual void				SetModfyStyle(eMarkerModifyStyle style) = 0;
		virtual eMarkerModifyStyle	GetModfyStyle() = 0;

		virtual void				SetStartCap(eMarkerCapStyle style, double size) = 0;
		virtual eMarkerCapStyle	GetStartCapStyle() = 0;
		virtual double				GetStartCapSize() = 0;

		virtual void				SetEndCap(eMarkerCapStyle style, double size) = 0;
		virtual eMarkerCapStyle	GetEndCapStyle() = 0;
		virtual double				GetEndCapSize() = 0;

		MarkerObj() {};
		virtual ~MarkerObj() {};
	};
	//****************************************************************************************************/
	class CImageViewMarkerEventHandler
	{
	public:
		virtual void OnMove(CImageViewEx* pWnd, MarkerObj* pObj) {};
		virtual void OnModify(CImageViewEx* pWnd, MarkerObj* pObj) {};
		virtual void OnMouseHover(CImageViewEx* pWnd, MarkerObj* pObj) {};
		virtual void OnMouseLeave(CImageViewEx* pWnd, MarkerObj* pObj) {};
		virtual void OnSelect(CImageViewEx* pWnd, MarkerObj* pObj) {};
	};

	class CImageViewEventHandler
	{
	public:
		//**For Mouse Event/////////////////////////////////////////////////////
		virtual void OnMouseMove(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseMove(CImageViewEx* pWnd, LONG nX, LONG nY, BOOL bLBtnDown, BOOL bRBtnDown, BOOL bMBtnDown) {};
		virtual void OnMouseHover(CImageViewEx* pWnd) {};
		virtual void OnMouseLeave(CImageViewEx* pWnd) {};
		virtual void OnMouseLButtonDown(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseLButtonUp(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseLButtonDblClk(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseRButtonDown(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseRButtonUp(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseMButtonDown(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseMButtonUp(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseWheelPos(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnMouseWheelNeg(CImageViewEx* pWnd, LONG nX, LONG nY) {};
		virtual void OnSelectingArea(CImageViewEx* pWnd, LPRECT prcArea) {};
		virtual void OnSelectedArea(CImageViewEx* pWnd, LPRECT prcArea) {};
		virtual void OnUpdatedView(CImageViewEx* pWnd) {};
		virtual void OnChangedImageInfo(CImageViewEx* pWnd) {};
		virtual void OnChangedImageZoomRatio(CImageViewEx* pWnd) {};
		virtual void OnLoadedImageFromFile(CImageViewEx* pWnd) {};
		virtual void OnUpdatedImage(CImageViewEx* pWnd) {};
		////////////////////////////////////////////////////////////////////////
	};

	typedef struct tagIMAGE_INFO
	{
		LONG nWidth;
		LONG nHeight;
		LONG nPitch;
		LONG nBitPP;
		LONG nBytePP;
		size_t _ImageSizeInByte;
	}IMAGE_INFO, LPIMAGE_INFO;


	class LT_LIB_CLASS CImageViewEx : public CWnd
	{
		DECLARE_DYNAMIC(CImageViewEx)
	private:
		class CImageMemDC;

	public:
		enum class eDispScaleType
		{
			ScaleToAuto = 0,
			ScaleToFit = 1
		};

		enum class eImageFmt
		{
			Min = 0,
			BMP = Min,
			GIF, JPG, PNG, MNG, ICO,
			TIF, TGA, PCX, WBMP, WMF,
			JP2, JPC, PGX, PNM, RAS,
			SKA, PSD, MaxCnt,
			Auto
		};

		CImageViewEx();
		virtual ~CImageViewEx();

		virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

		LPCTSTR GetFormatExt(eImageFmt fmt);
		eImageFmt GetFormatFromExt(LPCTSTR szExt);

		BOOL LoadImage(LPCTSTR szPath);
		BOOL LoadImageFromFileOpenDialog(size_t _count, ...);

		BOOL SaveImage(LPCTSTR szPath, eImageFmt fmt = eImageFmt::Auto);
		BOOL SaveRectImage(LPCTSTR szPath, LPRECT prc, eImageFmt fmt = eImageFmt::Auto);
		//BOOL SaveImageWithOverlay(LPCTSTR szPath, eImageFmt fmt = eImageFmt::Auto);
		BOOL SaveImageFromFileOpenDialog(size_t _count, ...);
		BOOL SaveOverlayImage(LPCTSTR szPath, LPRECT prcSize = NULL);

		//Update Image Functions for Original Image Array (means without pitch)
		BOOL UpdateImageFromArray(PBYTE pData, BOOL bFlip = FALSE);
		BOOL UpdateImageFromArray(PBYTE pData, LONG nWidth, LONG nHeight, WORD wBpp = 8, BOOL bFlip = FALSE);
		BOOL UpdateImageRectFromArray(PBYTE pRectArray, LPRECT prc, BOOL bFlip = FALSE);
		//////////////////////////////////////////////////////////////////////////////////

		//Update Image Functions for Bitmap Image Array (means with pitch)
		BOOL UpdateImageFromBitmapArray(PBYTE pData, BOOL bFlip = FALSE);
		BOOL UpdateImageFromBitmapArray(PBYTE pData, LONG nSrcPitch, LONG nDstPitch, BOOL bFlip);
		BOOL UpdateImageFromBitmapArray(PBYTE pData, LONG nWidth, LONG nPitch, LONG nHeight, WORD wBpp = 8, BOOL bFlip = FALSE);
		//BOOL UpdateImageRectFromBitmapArray(PBYTE pRectBmpArray, LPRECT prc, BOOL bFlip = FALSE);
		//////////////////////////////////////////////////////////////////////////////////

		//Remove All Data. means Inital ImageView
		BOOL RemoveImage();

		void SetDispScaleType(eDispScaleType type) { m_eScaleType = type; }
		eDispScaleType GetDispScaleType() { return m_eScaleType; }

		LONG GetWidth() { return m_nImageWidth; };
		LONG GetHeight() { return m_nImageHeight; };
		LONG GetPitch() { return m_nPitch; };
		LONG GetBitPerPixel() { return (LONG)m_wBitPerPixels; };
		LONG GetBytePerPixel() { return (LONG)m_wBytePerPixels; };
		BOOL IsLoadedImage() { return m_pImage ? TRUE : FALSE; };

		RECT GetImageRect();
		size_t GetImageSizeInByte();
		BOOL GetImage(void* pImage, BOOL bFlip = FALSE);

		SIZE_T GetBitmapImageSizeInByte();
		BOOL GetBitmapImage(void* pImage);

		COLORREF GetPixelValueRGB(LONG nX, LONG nY);
		BYTE GetPixelValueGray(LONG nX, LONG nY);

		//**For Overlay//////////////////////////////////////////////////////////////////
		void SetPen(int nPenStyle = PS_SOLID, int nPenWidth = 1);
		void DrawCenterCross(COLORREF crColor = RGB(0, 0, 255), double dLineWidthRatio = 0.0); //Use Overlay
		void DrawCross(LPPOINT ppt, LONG nSizeX, LONG nSizeY, COLORREF crColor = RGB(0, 0, 0));
		void DrawCross(LPRECT prc, COLORREF crColor = RGB(0, 0, 0));

		void DrawRectangle(LPRECT prc, COLORREF crColor = RGB(0, 0, 0), BOOL bFill = FALSE, COLORREF crFillColor = RGB(0, 0, 0));
		void DrawEllipse(LPRECT prc, COLORREF crColor = RGB(0, 0, 0), BOOL bFill = FALSE, COLORREF crFillColor = RGB(0, 0, 0));
		void DrawLine(LPPOINT ptStart, LPPOINT ptEnd, COLORREF crColor = RGB(0, 0, 0));
		void DrawPolygon(LPPOINT ptArray, int nArrayCnt, COLORREF crColor = RGB(0, 0, 0), BOOL bFill = FALSE, COLORREF crFillColor = RGB(0, 0, 0), BOOL bWinding = FALSE);

		void DrawText(LPCTSTR szText, int nChCount, LPRECT prc, COLORREF crColor = RGB(0, 0, 0), UINT nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER
			, LPCTSTR szFont = _T("Arial"), int nHeight = 10, int nWeight = FW_NORMAL);

		void DrawOverlayBinaryBitmapImage(PBYTE pImage, LPRECT prcDest, LONG nPitch, COLORREF crDrawColor, BYTE bytDestVal = 0);
		void DrawOverlayBinaryImage(PBYTE pImage, LPRECT prcDest, COLORREF crDrawColor, BYTE bytDestVal = 0);

		IMAGE_INFO GetOverlayBitmapImageInfo_Binary();
		IMAGE_INFO GetOverlayBitmapImageInfo_Color();

		BOOL GetOverlayImage_BitmapBinary(PBYTE pBuffer, COLORREF crDst, BYTE fore = 0, BYTE bk = 255);
		BOOL GetOverlayImage_BitmapColor(PBYTE pBuffer);

		BOOL GetOverlayImage_2DBinary(PBYTE pBuffer, COLORREF crDst, BYTE fore = 0, BYTE bk = 255);
		BOOL GetOverlayImage_2DColor(PBYTE pBuffer);

		HDC GetOverlayDC() { return m_OverlayDc.GetSafeHdc(); };
		COLORREF GetOverlayTransparentColor() { return m_OverlayDc.GetFillColor(); };

		void ClearOverlay(BOOL bIncludeMarker = TRUE);
		void SetOverlayAlpahValue(BYTE val, BOOL bRepaint = FALSE);
		BYTE GetOverlayAplahValue();
		/////////////////////////////////////////////////////////////////////////////////

		//**For Overlap//////////////////////////////////////////////////////////////////
		void DrawOverlapImage_DstBin(PBYTE pImage, LPRECT prc, double dDestVal, COLORREF crDest, BOOL bFlip = FALSE, BYTE bytAlphaVal = 255);
		void DrawOverlapImage(PBYTE pImage, LPRECT prc, WORD wBpp = 8, BOOL bFlip = FALSE, BYTE bytAlphaVal = 255);
		void RemoveOverlapImage();
		/////////////////////////////////////////////////////////////////////////////////

		//**For Marker///////////////////////////////////////////////////////////////////
		MarkerObj* AddMarkerRectangle(LPCTSTR szObjName, LPRECT prc, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix);

		MarkerObj* AddMarkerLine(LPCTSTR szObjName, LPPOINT ptStart, LPPOINT ptEnd, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix);

		MarkerObj* AddMarkerCross(LPCTSTR szObjName, LPPOINT ppt, LONG nSizeX, LONG nSizeY, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix);
		MarkerObj* AddMarkerCrossD(LPCTSTR szObjName, MarkerObj::LPPOINTD ppt, LONG nSizeX, LONG nSizeY, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix);

		MarkerObj* AddMarkerEllipse(LPCTSTR szObjName, LPRECT prc, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix);
		MarkerObj* AddMarkerPolygon(LPCTSTR szObjName, LPPOINT ptArray, size_t _ArrayCnt, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix);
		MarkerObj* AddMarkerPolyLine(LPCTSTR szObjName, LPPOINT ptArray, size_t _ArrayCnt, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix);

		MarkerObj* AddMarkerText(LPCTSTR szObjName, LPPOINT ppt
			, LPCTSTR szText, int nTextHeight = 10, int nTextWeight = FW_NORMAL, LPCTSTR szTextFace = _T("Tahoma")
			, eMarkerTextAlign TextAlign = eMarkerTextAlign::LEFT, eMarkerModifyStyle eModifyStyle = eMarkerModifyStyle::Fix
			, UINT nDrawQuality = NONANTIALIASED_QUALITY);

		MarkerObj* GetMarker(LPCTSTR szObjName);

		void RemoveMarker(LPCTSTR szObjName);
		void RemoveAllMarker();
		/////////////////////////////////////////////////////////////////////////////////

		//**For Label////////////////////////////////////////////////////////////////////
		LabelObj* AddLabelText(LPCTSTR szObjName, LPPOINT ppt
			, LPCTSTR szText, int nTextHeight = 10, int nTextWeight = FW_NORMAL, LPCTSTR szTextFace = _T("Tahoma")
			, eLabelTextAlign TextAlign = eLabelTextAlign::LEFT, eLabelModifyStyle eModifyStyle = eLabelModifyStyle::Fix
			, UINT nDrawQuality = NONANTIALIASED_QUALITY);

		LabelObj* GetLabel(LPCTSTR szObjName);
		void RemoveLabel(LPCTSTR szObjName);
		void RemoveAllLabel();
		/////////////////////////////////////////////////////////////////////////////////

		//**Zoom Control/////////////////////////////////////////////////////////////////
		void SetZoomRatio(double dZoomRatio = 100.0, BOOL bRePaint = TRUE);
		void ResetZoomRatio(BOOL bRePaint = TRUE);
		double GetZoomRatio();

		double GetMinimumZoomRatio();
		double GetMaximumZoomRatio();

		void SetMinimumZoomRatio(double dVal, BOOL bRepaint = TRUE);
		void SetMaximumZoomRatio(double dVal, BOOL bRepaint = TRUE);

		double GetZoomInOutUnit();
		void SetZoomInOutUnit(double dVal);
		/////////////////////////////////////////////////////////////////////////////////

		//**View Control/////////////////////////////////////////////////////////////////
		void SetViewPosition(LPPOINT ppt, BOOL bRePaint = TRUE);
		void ResetViewPosition(BOOL bRePaint = TRUE);
		POINT GetViewPosition();
		/////////////////////////////////////////////////////////////////////////////////

		//**Set Mode////////////////////////////////////////////////////////////////////
		void EnablePopupMenu(BOOL bEnable) { m_bEnablePopup = bEnable; };
		BOOL GetEnablePopupMenu() { return m_bEnablePopup; };

		void EnableWheelMode(BOOL bEnable) { m_bEnableWheelMode = bEnable; };
		BOOL GenEnableWheelMode() { return m_bEnableWheelMode; };

		void EnableWheelZoomMousePos(BOOL bEnable) { m_bEnableWheelZoomMousePos = bEnable; }
		BOOL GetEnableWheelZoomMousePos() { return m_bEnableWheelZoomMousePos; }

		void EnableDrawPixelInfo(BOOL bEnable) { m_bDrawPixelInfo = bEnable; };
		BOOL GetEnableDrawPixelInfo() { return m_bDrawPixelInfo; };

		void EnableDrawPixelGraphHor(BOOL bEnable) { m_bDrawPixelGraph_Hor = bEnable; };
		BOOL GetEnableDrawPixelGraphHor() { return m_bDrawPixelGraph_Hor; };

		void EnableDrawPixelGraphVer(BOOL bEnable) { m_bDrawPixelGraph_Ver = bEnable; };
		BOOL GetEnableDrawPixelGraphVer() { return m_bDrawPixelGraph_Ver; };

		void SetWindowHatchStyle(eHatchStyle style) { m_eWindowHatchStyle = style; };
		eHatchStyle GetWindowHatchStyle() { return m_eWindowHatchStyle; };

		void SetWindowForeColor(COLORREF cr) { m_crWindowForeColor = cr; };
		COLORREF GetWindowForeColor() { return m_crWindowForeColor; };

		void SetWindowFillColor(COLORREF cr) { m_crWindowFillColor = cr; };
		COLORREF GetWindowFillColor() { return m_crWindowFillColor; };

		void EnableSelectedArea(BOOL bEnable) { m_bEnableSelectedArea = bEnable; }
		BOOL GetEnableSelectedArea() { return m_bEnableSelectedArea; }

		void EnableDispDragMode(BOOL bEnable) { m_bEnableDispDragMode = bEnable; }
		BOOL GetEnableDispDragMode() { return m_bEnableDispDragMode; };

		void EnableCenterCrossLine(BOOL bEnable, COLORREF crColor);
		BOOL GetEnableCenterCrossLine();
		COLORREF GetCenterCrossLineColor();
		////////////////////////////////////////////////////////////////////////////////

		//**Set Marekr Event Handler////////////////////////////////////////////////////
		void SetMarkerEventHandler(CImageViewMarkerEventHandler* pMarkerEvtHandler);
		////////////////////////////////////////////////////////////////////////////////

		//**Set Marekr Event Handler////////////////////////////////////////////////////
		void SetEventHandler(CImageViewEventHandler* pEventHandler);
		////////////////////////////////////////////////////////////////////////////////

		RECT GetSelectedArea();
		void ClearSelectedArea();

		//**Display Refresh depends on Message queue
		BOOL Invalidate(BOOL bErase = TRUE);

		//**Display wiil be immediately Refresh
		BOOL InvalidateDirect(BOOL bErase = TRUE);

	protected:
		DECLARE_MESSAGE_MAP()

	public:

		class CPointEx : public POINT
		{
		public:
			double dx;
			double dy;
			CPointEx(POINT& pt);
			CPointEx(int _x, int _y);
			CPointEx(double _x, double _y);
			CPointEx();
			~CPointEx();

			void operator=(const POINT& pt);
			void operator=(const CPointEx& pt);
			bool operator==(const POINT& pt);
			bool operator==(const CPointEx& pt);
			bool operator!=(const POINT& pt);
			bool operator!=(const CPointEx& pt);

			void SetPoint(int _x, int _y);
			void SetPoint(double _x, double _y);

			POINT GetPoint();
		};
		class CRectEx
		{
		public:
			double left;
			double right;
			double top;
			double bottom;
			CRectEx(const CRectEx& rc);
			CRectEx(const CRect& rc);
			CRectEx();
			CRectEx(double _left, double _top, double _right, double _bottom);
			~CRectEx();

			void SetRect(double _left, double _top, double _right, double _bottom);
			bool IsRectEmpty();
			double Width();
			double Height();
			void SetRectEmpty();
			CPointEx CenterPoint();
			void InflateRect(double x, double y);
			void DeflateRect(double x, double y);
			bool PtInRect(CPointEx& pt);
			bool PtInRect(LPPOINT ppt);
			bool PtInRect(POINT& pt);
			bool PtInEllipse(CPointEx& pt);
			bool PtInEllipse(LPPOINT ppt);
			bool PtInEllipse(POINT& pt);
			bool RectInLine(LPPOINT ppt1, LPPOINT ppt2);
			bool RectInLine(CPointEx& ppt1, CPointEx& ppt2);
			void operator=(const RECT& rc);
			void operator=(const CRectEx& rc);
			void MoveToXY(double x, double y);
		};
	private:
		class CImageMemDC
		{
		public:
			CImageMemDC();
			virtual ~CImageMemDC();

		private:
			HDC m_hdc;
			BITMAPINFO* m_pbmi;
			HBITMAP m_hBm;
			HBITMAP m_hOldBm;
			COLORREF m_crFillColor;
			COLORREF m_crForeColor;
			int m_nHatchStyle;

			void Destroy();

		public:
			HDC CreateComaptibleDC(HDC hdc);
			HBITMAP MakeBitmap(long nWidth, long nHeight, WORD wBpp);
			HBITMAP MakeBitmap(long nWidth, long nHeight, long nPitch, WORD wBpp);
			HBITMAP MakeBitmap(LPBITMAPINFO lpbmi);
			//**For Binary Overlaps//////////////////////////////////
			HBITMAP MakeBitmap_ForBin(long nWidth, long nHeight, BYTE bytDestVal, COLORREF crDest, COLORREF crBk);
			HBITMAP MakeBitmap_ForBin(long nWidth, long nHeight, long nPitch, BYTE bytDestVal, COLORREF crDest, COLORREF crBk);
			/////////////////////////////////////////////////////////
			BOOL ClearBitmap();
			BOOL ClearBitmap(COLORREF crFillColor);
			BOOL ClearBitmap(int nHatchStyle, COLORREF crFill, COLORREF crFore);
			BOOL UpdateBitmapImage(PBYTE pData);
			HDC GetSafeHdc() { return m_hdc; }
			COLORREF GetFillColor() { return m_crFillColor; }
			const LPBITMAPINFO GetBitmapInfo() { return (const LPBITMAPINFO)m_pbmi; }
			long GetBitmapWidth() { return m_pbmi ? m_pbmi->bmiHeader.biWidth : 0; }
			long GetBitmapHeight() { return m_pbmi ? abs(m_pbmi->bmiHeader.biHeight) : 0; }
			BOOL IsValid();

			size_t GetBitmapImageSize();
			size_t GetBitmapImagePitch();
			BOOL GetBitmapImageBuffer(PBYTE pBuffer);
			HBITMAP GetHBitmap() { return m_hBm; }

			size_t GetGrayBitmapImageSize();
			size_t GetGrayBitmapImagePitch();
			BOOL GetGrayBitmapImageBuffer(PBYTE pBuffer, BOOL bConvertBin = FALSE, COLORREF crBinDst = RGB(0, 0, 0), BYTE binFore = 0, BYTE binBk = 255);
		};

		PBYTE m_pImage;
		LONG m_nImageWidth;
		LONG m_nImageHeight;
		WORD m_wBitPerPixels;
		WORD m_wBytePerPixels;
		LONG m_nDummyByte;
		LONG m_nPitch;

		//Control 전체를 의미한다.
		CImageMemDC m_WindowDc;
		CRect m_rcWindowRect;

		CImageMemDC m_MarkerDC;
		////////////////////////////

		//Window를, Image 영역, Menu 영역, StatusBar 영역, H Scroll 영역, V Scroll 영역으로 나누어야 한다.
		//우선 Image Draw Area 만 구현토록 한다.
		CRect m_rcImageDrawArea;
		/////////////////////////////////////////////////////////////////////

		CRect m_rcScaledImageDrawArea;			//이미지와 Overlay 만 그려질 영역
		CRect m_rcDrawingImageRect;				//Zoom 에 따른, 그리고자 하는 이미지의 크기.

		CImageMemDC m_ImageDc;
		CImageMemDC m_OverlayDc;
		CImageMemDC m_OverlayBk;
		BYTE		m_bytOverayAlpah;

		eDispScaleType m_eScaleType;

		double m_dZoomRatio;
		double m_dZoomRatio_Max;
		double m_dZoomRatio_Min;
		double m_dZoomInOutUnit;

		CPointEx m_ptMousePosInImage;
		CPointEx m_ptDrawwingPosInImage;

		BOOL m_bMouseDown;
		CPointEx m_ptMouseDownPoint;
		CPointEx m_ptMouseDownCenterPtInImage;

		BOOL m_bInMarkerEdge;
		BOOL m_bInMarkerMiddleEdge;
		BOOL m_bInMarkerArea;

		BOOL m_bMouseRDown;
		CPointEx m_ptMouseRDownPoint;
		CPointEx m_ptMouseRDownPosInImage;

		BOOL m_bMouseMDown;
		CPointEx m_ptMouseMDownPoint;
		CPointEx m_ptMouseMDownPosInImage;

		BOOL SetMenu();
		BCMenu m_menuPopup;
		void OnCommandImageOpen();
		void OnCommandImageSave();
		void OnCommandZoomIn();
		void OnCommandZoomOut();
		void OnCommandAutoScale();
		void OnCommandScaleToFit();
		void OnCommandClearOverlay();
		void OnCommandPixelInfo();
		afx_msg void OnUpdatePixelInfo(CCmdUI* pCmdUI);

		void OnCommandPixelGraph_Hor();
		afx_msg void OnUpdatePixelGraph_Hor(CCmdUI* pCmdUI);
		void OnCommandPixelGraph_Ver();
		afx_msg void OnUpdatePixelGraph_Ver(CCmdUI* pCmdUI);
		void OnCommandPixelGraphCh_Gray();
		afx_msg void OnUpdatePixelGraphCh_Gray(CCmdUI* pCmdUI);
		void OnCommandPixelGraphCh_R();
		afx_msg void OnUpdatePixelGraphCh_R(CCmdUI* pCmdUI);
		void OnCommandPixelGraphCh_G();
		afx_msg void OnUpdatePixelGraphCh_G(CCmdUI* pCmdUI);
		void OnCommandPixelGraphCh_B();
		afx_msg void OnUpdatePixelGraphCh_B(CCmdUI* pCmdUI);
		void OnCommandAnalysis_Histogram();
		void OnCommandAnalysis_ProfileX();
		void OnCommandAnalysis_ProfileY();

		void GetMousePosInImage(CPoint& ptClientPos, CPointEx& ptImagePos);
		void CalcurateAreaRect();
		void CalcurateAreaRect(
			IN CImageMemDC& windowDC
			, IN CRect& rcWindowRect
			, IN eDispScaleType eScaleType
			, IN double dZoomRatio
			, OUT CRect& rcImageDrawArea
			, OUT CRect& rcDrawingImageRect
			, OUT CPointEx& ptDrawwingPosInImage
			, OUT CRect& rcScaledImageDrawArea
			, OUT double& dPtPerPixel_X
			, OUT double& dPtPerPixel_Y
			, OUT double& dPixelPerPt_X
			, OUT double& dPixelPerPt_Y
			, OUT CRect& rcMarkerDC
			, OUT CRect& rcDrawingMarkerArea
			, OUT CImageMemDC& markerDC);

		BOOL m_bFlip;
		CCriticalSection m_cs;

		//**ForMarker//////////////////////////////////////
		std::list<MarkerObj*> m_listMarker;
		CImageViewMarkerEventHandler* m_pMarkerEventHandler;

		CRect m_rcMarkerDC;
		CRect m_rcDrawingMarkerArea;
		double m_dPtPerPixel_X;
		double m_dPtPerPixel_Y;
		double m_dPixelPerPt_X;
		double m_dPixelPerPt_Y;
		//CPointEx MarkerPosToImagePos(CPointEx* ptMarkerPos);
		//CPointEx ImagePosToMarkerPos(CPointEx* ptImagePos);

		MarkerObj* m_pCurSelObj;
		size_t	m_EdgeIdx;
		std::vector<CPointEx> m_vtMouseDownMarkerPoint;
		CRect m_rcMouseDownMarkerArea;
		HCURSOR m_hCursor;
		MarkerObj* GetMarkerObjFromImagePos(CPointEx* ptImagePos);
		BOOL IsPosInMarkersEdge(MarkerObj* pObj, CPointEx* ptImagePos, size_t* pEdgeIdx = NULL);
		BOOL IsPosInMarkersMiddleEdge(MarkerObj* pObj, CPointEx* ptImagePos, size_t* pEdgeIdx = NULL);
		HCURSOR LoadMarkEdgeCursor(MarkerObj* pObj, size_t _CurrEdgeIdx);
		HCURSOR LoadMarkMiddleEdgeCursor(MarkerObj* pObj, size_t _CurrEdgeIdx);
		void SetValidRangeMarker(MarkerObj* pObj);
		///////////////////////////////////////////////////

		//**ForLabel/////////////////////////////////////////
		std::list<LabelObj*> m_listLabel;
		/////////////////////////////////////////////////////

		void DrawMarker();
		void DrawMarker(
			IN CImageMemDC& windowDC
			, IN CImageMemDC& markerDC
			, IN  CRect& rcSelectedArea
			, IN  CRect& rcScaledImageDrawArea
			, IN  CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y);

		void DrawMarker_Rectangle(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);

		void DrawMarker_Cross(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);

		void DrawMarker_Line(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);

		void DrawMarker_Ellipse(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);
		void DrawMarker_Polygon(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);
		void DrawMarker_PolyLine(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);
		void DrawMarker_Text(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);
		void DrawMarker_CrossD(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y
			, MarkerObj* pObj);

		void DrawCenterCrossLine(
			IN CImageMemDC& MarkerDC
			, IN CRect& rcDrawingImageRect
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y
			, IN double& dPixelPerPt_X
			, IN double& dPixelPerPt_Y); //UseMarker

		void DrawLabel();
		void DrawLabel_Text(LabelObj* pObj);

		void SaveBitmapToDirectFile(CDC* pDC, CRect BitmapSize, int BitCount, CString strFilePath);

		CCriticalSection m_csDraw;
		CCriticalSection m_csUpdateDraw;
		BOOL m_bDoUpdate;
		//**For Overlay/////////////////////////////////////
		BOOL m_bIsUsedOverlay;
		int m_nPenStyle;
		int m_nPenWidth;
		CCriticalSection m_csOverlay;
		////////////////////////////////////////////////////

		//**For Mode////////////////////////////////////////
		BOOL m_bEnablePopup;
		BOOL m_bEnableWheelMode;
		BOOL m_bEnableWheelZoomMousePos;
		BOOL m_bEnableSelectedArea;
		BOOL m_bEnableDispDragMode;

		eHatchStyle m_eWindowHatchStyle;
		COLORREF m_crWindowForeColor;
		COLORREF m_crWindowFillColor;
		////////////////////////////////////////////////////

		//**Event Handler//////////////////////////////////
		CImageViewEventHandler* m_pEventHandler;
		BOOL m_bIsMouseImageHover;
		///////////////////////////////////////////////////

		//**Info Display////////////////////////////////////
		BOOL m_bDrawPixelInfo;
		BOOL m_bDrawPixelGraph_Hor;
		BOOL m_bDrawPixelGraph_Ver;

		enum class ePixelGraphCh
		{
			Gray,
			R,
			G,
			B
		};

		ePixelGraphCh m_eDrawPixelGraphCh;
		////////////////////////////////////////////////////

		//**For Center Cross Line///////////////////////////
		BOOL m_bEnableCenterCrossLine;
		COLORREF m_crCenterCrossLine;
		////////////////////////////////////////////////////

		void DrawPixelInfo();
		void DrawPixelGraph();

		BOOL m_bIsSelectingMode;
		CRect m_rcSelectedArea;
		void DrawSelectedArea();
		void DrawSelectedArea(
			IN CImageMemDC& WindowDc
			, IN CImageMemDC& MarkerDC
			, IN CRect& rcSelectedArea
			, IN CRect& rcDrawingImageRect
			, IN CRect& rcScaledImageDrawArea
			, IN double& dPtPerPixel_X
			, IN double& dPtPerPixel_Y);

		typedef struct tagOVERLAPIMG
		{
			CImageMemDC dc;
			CImageMemDC dcBK;
			CRect rcRect;
			BOOL bIsBin;
			BYTE bytAlphaVal;
		}OVERLAPIMG, *LPOVERLAPIMG;

		LPOVERLAPIMG m_pOverlapImg;



		void CalcurateZoomUnit();

	protected:
		LRESULT OnUpdateUI(WPARAM wParam, LPARAM lParam);
		virtual void PreSubclassWindow();
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseHover(UINT nFlags, CPoint point);
		afx_msg void OnMouseLeave();
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	public:
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	};
}
