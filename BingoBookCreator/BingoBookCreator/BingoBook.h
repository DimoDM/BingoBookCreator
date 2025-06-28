#pragma once
#include "hpdf.h"
#include <map>
#include <string>

class BingoBook
{
  public:
	BingoBook( size_t numOfPages = 0, size_t bingoSheetsOnPage = 0, const std::string &location = "" );

	void SetNumOfPages( size_t numOfPages = 0 );
	void SetSheetsOnPage( size_t bingoSheetsOnPage = 0 );
	void SetPDFLocation( const std::string &location = "" );
	bool IsAbleToCreate();
	bool CreatePDF();

  private:
	struct TableInfo
	{
		TableInfo( float t, float l, float c, int fs, float hX, float hY, float nX, float nY )
			: top( t ), left( l ), cellSize( c ), fontSize( fs ), hXMargin( hX ), hYMargin( hY ), nXMargin( nX ), nYMargin( nY )
		{
		}

		float top = 0.0f;
		float left = 0.0f;
		float cellSize = 0.0f;
		float hXMargin = 15.0f;
		float hYMargin = -1.5f;
		float nXMargin = 13.0f;
		float nYMargin = 8.0f;
		int	  fontSize = 0;
	};

#ifdef _WIN64
	wchar_t *GetSaveFileNameFromDialog();
#endif
	std::string GetFileLocation();
	void		AddTable( HPDF_Doc pdf, HPDF_Page page, const TableInfo &tInfo );

  private:
	size_t		numOfPages = 0;
	size_t		bingoSheetsOnPage = 0;
	std::string location;
};