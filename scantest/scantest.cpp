// scantest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "scan.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	init_scan();

	/*	Test PDF Scan	*/
	//open_doc("Scan.pdf");
	if (1 == rectify_scan("scan_p1.jpg"))
		std::cout << "Scan data extracted." << std::endl;
	else
		std::cout << "Erreur d'extraction de scan" << std::endl;

	//close_doc();


	/*	Test JPEG Scan
	std::ofstream correction("S.txt");
	
	for (size_t i = 1; i <= 35; i++)
	{
		std::stringstream fname;
		fname << "S" << std::setfill('0') << std::setw(2) << i << ".jpg" << std::ends;

		if (1 == open_scan(fname.str().c_str()))
		{
			std::ios state(nullptr);
			state.copyfmt(correction);
			correction << std::setfill('0') << std::setw(2) << i << ";-;" << std::endl;
			correction.copyfmt(state);
		}
		else
		{
			extract_scan_fills(20, DEFAULT_NUM_COLUMNS);

			//	Export scan in correction file
			SCAN scan;
			scan.scan_id = i;
			scan.num_answers = 20;
			scan.answers = new QCM_Answer[20];

			if (1 == get_scan_answers(&scan))
			{
				std::ios state(nullptr);
				state.copyfmt(correction);
				correction << std::setfill('0') << std::setw(2) << scan.scan_id << ";";
				correction.copyfmt(state);

				for (size_t i = 0; i < scan.num_answers; i++)
				{
					QCM_Answer qanswer = scan.answers[i];
					unsigned char answer = qanswer.question_value;

					char value = (answer == 5 ? 'e' : (answer == 4 ? 'd' : (answer == 3 ? 'c' : (answer == 2 ? 'b' : (answer == 1 ? 'a' : ' ')))));
					correction << value << ";";
				}
				correction << std::endl;
			}
		}
	}

	correction.close();

	close_scan();
	*/

	release_scan();

	return 0;
}

