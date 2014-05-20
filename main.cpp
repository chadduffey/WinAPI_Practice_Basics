/*  
 * author/id: Chad Duffey 
 * purpose: Learning Keyboard and Mouse input
 * date created: 10/08/2011
 * date last Modified: 10/08/2011
 */

#include <windows.h>
#include <time.h>


/*Program Functions */

//checks if the user is moving the rectangle to a valid location
bool validMove(int L, int R, int T, int B, RECT &clientarea, int direction);

//moves the rectangle to a random location
RECT randomSpot(RECT &clientarea);												

//determines if the mouse is over the rectangle
bool mouseOverRectangle(int mouseXPos, int mouseYPos, RECT myRect);

/*  Declare Windows procedure  */
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "Assignment1-Q1";

/* Global Constants */
const int MOVEAMOUNT	= 2;	//how the rectangle should jump each directional keystroke
const int RECTWIDTH		= 250;	//width of the rectangle
const int RECTHEIGHT	= 120;	//hight of the rectangle


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;			//Window Handle
    MSG messages;		//messages saved
    WNDCLASSEX wincl;	//Window Class

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WndProc;  /* Message handler */
    wincl.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;   /* No menu */
    wincl.cbClsExtra = 0;        
    wincl.cbWndExtra = 0;  
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOW;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   
           szClassName,							/* Classname */
           "Assignment1, Keyboard and Mouse",	/* Title Text */
           WS_OVERLAPPEDWINDOW,					/* default window */
           CW_USEDEFAULT,						/* Windows position */
           CW_USEDEFAULT,						/* on the screen */
           544,									/* Width */
           375,									/* Height, in pixels */
           HWND_DESKTOP,						/* Parent window */
           NULL,								/* No menu */
           hThisInstance,						/* Program Instance handler */
           NULL									/* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    return messages.wParam;
}


/*  This function is called by the function DispatchMessage() line 69 */
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;						// Handle to a device context, for drawing
    PAINTSTRUCT ps;					// Extra information useful when painting
    
	static HBRUSH	brush1;			//for colour bonus feature
	RECT client, rect1;				//rectangles

	static int rectLeft		= 20;	//hold coordinates of moving rectangle
	static int rectTop		= 20;
	static int rectRight	= RECTWIDTH;
	static int rectBottom	= RECTHEIGHT;

    switch (message)                  /* handle the messages */
    {
        //-------------------------------------------
        case WM_CREATE:
		{     
			// Once only initialisation
			
			/* initialize random seed: */
			srand ( time(NULL) );

			//create brush one
			COLORREF colour1 = RGB(30,20,20);
			brush1 = CreateSolidBrush(colour1);

			return 0;
		}
        //-------------------------------------------        
        case WM_KEYDOWN: 
			
			RECT rClientRect;							//used to hold current client area coordinates
			GetClientRect(hwnd, (LPRECT)&rClientRect);	//update with current client area coordinates

			switch (wParam) 
            { 
                case VK_LEFT: 
                    
                    // Process the LEFT ARROW key. 
					if (validMove(rectLeft-MOVEAMOUNT,rectRight-MOVEAMOUNT,rectTop,rectBottom,rClientRect,1))
					//only perform move if the desireed move is inside the client area completely
					{
						rectRight	-= MOVEAMOUNT;
						rectLeft	-= MOVEAMOUNT;
						InvalidateRect(hwnd,NULL,TRUE);
					}
					break; 
 
                case VK_RIGHT: 
                    
                    // Process the RIGHT ARROW key. 
                    if (validMove(rectLeft+MOVEAMOUNT,rectRight+MOVEAMOUNT,rectTop,rectBottom,rClientRect,2))
					//only perform move if the desireed move is inside the client area completely
					{
						rectRight	+= MOVEAMOUNT;
						rectLeft	+= MOVEAMOUNT;
						InvalidateRect(hwnd,NULL,TRUE);
					}
                    break; 
 
                case VK_UP: 
                    
                    // Process the UP ARROW key. 
					if (validMove(rectLeft,rectRight,rectTop-MOVEAMOUNT,rectBottom-MOVEAMOUNT,rClientRect,3))
					//only perform move if the desireed move is inside the client area completely
					{
						rectTop		-= MOVEAMOUNT;
						rectBottom	-= MOVEAMOUNT;
						InvalidateRect(hwnd,NULL,TRUE);
					}
					break; 
 
                case VK_DOWN: 
                    
                    // Process the DOWN ARROW key. 
                    if (validMove(rectLeft,rectRight,rectTop+MOVEAMOUNT,rectBottom+MOVEAMOUNT,rClientRect,4))
					//only perform move if the desireed move is inside the client area completely
					{
						rectTop		+= MOVEAMOUNT;
						rectBottom	+= MOVEAMOUNT;
						InvalidateRect(hwnd,NULL,TRUE);
					}
					break; 
 
                // Process other non-character keystrokes. 
                 
                default: 
                    break; 
            } 

		//-------------------------------------------
		case WM_MOUSEMOVE:
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			RECT myRect = {rectLeft, rectTop, rectRight, rectBottom};
			RECT clientRect;
			GetClientRect(hwnd, (LPRECT)&clientRect);

			if (mouseOverRectangle(iPosX, iPosY, myRect))
			//mouse has entered the rectange need to move it
			{
				brush1 = CreateSolidBrush(RGB(rand()%255, rand()%255, rand()%255)); //bonus feature only
																					//providing random color for brush
				RECT newRect = randomSpot(clientRect);
				rectLeft = newRect.left;
				rectRight = newRect.right;
				rectTop = newRect.top;
				rectBottom = newRect.bottom;
				InvalidateRect(hwnd,NULL,TRUE);
			}
		}

		//-------------------------------------------
		case WM_PAINT:
			hdc = BeginPaint( hwnd, &ps );
			GetClientRect(hwnd, &client);

			rect1.top = rectTop;
			rect1.bottom = rectBottom;
			rect1.left = rectLeft;
			rect1.right = rectRight;

			FillRect(hdc, &rect1, brush1);

			DrawText(hdc, TEXT("Chad, Click Me"), -1, &rect1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            EndPaint( hwnd, &ps );
            return 0;
             
       //-------------------------------------------
		
		
		case WM_DESTROY:
             // Once only cleanup.
            PostQuitMessage (0);       
			/* WM_QUIT  */
        return 0;
        
        default:
			/* for messages that we don't deal with */
			return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

bool validMove(int L, int R, int T, int B, RECT &clientarea, int direction)
/* Takes in the desired value for Left, Right, Top Bottom coordinates of rectangle to ensure it is inside the 
client area. a rectangle object is passed for the client area, and an integer to represent the direction pressed. */
{

	switch (direction)
	{
		case 1:
			//left
			if (L < 0)
			return false;
		case 2:
			//right
			if (clientarea.right - R < 0)
			return false;
		case 3:
			//up
			if (T < 0)
				return false;
		case 4:
			//down	
			if (clientarea.bottom - B < 0)
				return false;
		default:
			return true;
	}
}

RECT randomSpot(RECT &clientarea)
//return coordinates for a rectangle generated at random
//used in this program for the jumping mouse component of the application
{
	RECT randomRECT;
	
	int rightboundary	= clientarea.right;			//boundary of client area
	int bottomboundary	= clientarea.bottom;

	int maxRight = rightboundary - RECTWIDTH;		//if we take into account size of rectangle
	int maxBottom = bottomboundary - RECTHEIGHT;	//this is boundary where we can possibly start

	randomRECT.top = rand() % maxBottom;
	randomRECT.left = rand() % maxRight;
	randomRECT.bottom = randomRECT.top + RECTHEIGHT - 20; 
	randomRECT.right = randomRECT.left + RECTWIDTH - 20;

	return randomRECT;
}

bool mouseOverRectangle(int mouseXPos, int mouseYPos, RECT myRect)
//check if the mouse coordinates are over the rectangle
//accept the coordinates as the first two parameters and the rectangle structure
{
	if ((mouseXPos > myRect.left) && (mouseXPos < myRect.right))
	{
		if ((mouseYPos > myRect.top) && (mouseYPos < myRect.bottom))
		{
			//mouse is inside our rectangle
			return true;
		}
	}
	return false;
}




