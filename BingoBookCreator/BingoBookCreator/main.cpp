/*
#include <windows.h>
#include <commdlg.h>
#include <iostream>

#include <locale>
#include <codecvt>
*/


#include "BingoBook.h"

int main()
{
	BingoBook bingoBook;
	bingoBook.CreatePDF();

	return 0;
}


/*
#include <iostream>
#include <hpdf.h>

// Function to add a table to the PDF
void AddTable(HPDF_Doc pdf, HPDF_Page page) {
	// Table header
	const char* columns[] = {"ID", "Name", "Age"};
	float table_x = 50.0f;  // Starting position for the table
	float table_y = 750.0f; // Starting position for the table (from top)
	float cell_width = 100.0f; // Width of each column
	float cell_height = 20.0f; // Height of each row

	// Setting font and size
	HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_Page_SetFontAndSize(page, font, 12);

	// Draw table header
	for (int i = 0; i < 3; ++i) {
		HPDF_Page_Rectangle(page, table_x + i * cell_width, table_y, cell_width, cell_height);
		HPDF_Page_Stroke(page);
		HPDF_Page_BeginText(page);
		HPDF_Page_TextOut(page, table_x + i * cell_width + 5, table_y + 5, columns[i]);
		HPDF_Page_EndText(page);
	}

	// Data rows
	const char* rows[][3] = {
		{"1", "Viki", "28"},
		{"2", "Jane Smith", "34"},
		{"3", "Emily Johnson", "22"}
	};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			HPDF_Page_Rectangle(page, table_x + j * cell_width, table_y - (i + 1) * cell_height, cell_width, cell_height);
			HPDF_Page_Stroke(page);
			HPDF_Page_BeginText(page);
			HPDF_Page_TextOut(page, table_x + j * cell_width + 5, table_y - (i + 1) * cell_height + 5, rows[i][j]);
			HPDF_Page_EndText(page);
		}
	}
}

// Main function to create the PDF file
int main() {
	HPDF_Doc pdf = HPDF_New(NULL, NULL);
	if (!pdf) {
		std::cerr << "Error creating PDF document" << std::endl;
		return 1;
	}

	// Create a new page
	HPDF_Page page = HPDF_AddPage(pdf);

	// Set page size (A4)
	HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

	// Add table to the PDF
	AddTable(pdf, page);

	// Save the PDF to a file
	const char* output_file = "D:/Development/output_table.pdf";
	HPDF_SaveToFile(pdf, output_file);

	// Clean up and release resources
	HPDF_Free(pdf);

	std::cout << "PDF with table created successfully: " << output_file << std::endl;
	return 0;
}
*/