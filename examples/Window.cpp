#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// These mostly have the form SFGUI/<Widget name>.hpp
#include <SFGUI/Window.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Window Example" );

	// Create our main SFGUI window

	// Almost everything in SFGUI is handled through smart pointers
	// for automatic resource management purposes. You create them
	// and they will automatically be destroyed when the time comes.
	sfg::Window::Ptr window;

	// Here we do the actual creation of the window. Creation of
	// widgets is always done with it's Create() method.
	window = sfg::Window::Create();

	// Here we can set the window's title bar text.
	window->SetTitle( "Title" );

	// For more things to set around the window refer to the
	// API documentation.

	// Start the game loop
	while ( app_window.IsOpened() ) {
		// Process events
		sf::Event event;

		while ( app_window.PollEvent( event ) ) {
			// Every frame we have to send SFML events to the window
			// to enable user interactivity. Without doing this your
			// GUI is nothing but a big colorful picture ;)
			window->HandleEvent( event );

			// Close window : exit
			if(
				(event.Type == sf::Event::Closed) ||
				(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Escape)
			) {
				app_window.Close();
			}
		}

		// Clear screen
		app_window.Clear();

		// After drawing the rest of your game, you have to let the GUI
		// render itself. If you don't do this you will never be able
		// to see it ;)
		window->Expose( app_window );

		// NOTICE
		// Because the window doesn't have any children it will shrink to
		// it's minimum size of (0,0) resulting in you not seeing anything
		// except the title bar text ;) Don't worry, in the Label example
		// you'll get to see more.

		// Update the window
		app_window.Display();
	}

	return EXIT_SUCCESS;
}
