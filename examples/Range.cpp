#include <sstream>

#include <SFML/Graphics.hpp>

// Always include the necessary header files.
// These mostly have the form SFGUI/<Widget name>.hpp
#include <SFGUI/Window.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>

// Create our label smart pointer.
sfg::Label::Ptr label;

// Create our adjustment smart pointer.
sfg::Adjustment::Ptr adjustment;

void AdjustmentChange();

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window( sf::VideoMode( 800, 600 ), "SFGUI Range Example" );

	// Create our main SFGUI window
	sfg::Window::Ptr window;
	window = sfg::Window::Create();
	window->SetTitle( "Title" );

	// Create our box.
	sfg::Box::Ptr box = sfg::Box::Create( sfg::Box::Horizontal );

	// Create a label.
	label = sfg::Label::Create();
	label->SetText( "20" );

	// Scale and Scrollbar widgets are subclasses of the Range class.
	// They have a common data representation object known as an
	// Adjustment. The Adjustment that each range widget is bound to
	// determines it's current state (where the slider is, what max
	// and min values are, how much to scroll per step etc.). Because
	// range widgets share this common data object they can also be
	// linked together by a common Adjustment instance. An Adjustment
	// is created automatically for you when you create a range widget.

	// Create our scale smart pointer.
	sfg::Scale::Ptr scale;

	// Create the scale itself.
	// We want a horizontal scale.
	scale = sfg::Scale::Create( sfg::Scale::Horizontal );

	// Create our scrollbar smart pointer.
	sfg::Scrollbar::Ptr scrollbar;

	// Create the scrollbar itself.
	// We want a vertical scrollbar.
	scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Vertical );

	// We can link both widgets together by their adjustments.
	adjustment = scrollbar->GetAdjustment();
	scale->SetAdjustment( adjustment );

	// Tune the adjustment parameters.
	adjustment->SetLower( 20.f );
	adjustment->SetUpper( 100.f );

	// How much it should change when clicked on the stepper.
	adjustment->SetMinorStep( 3.f );

	// How much it should change when clicked on the trough.
	adjustment->SetMajorStep( 12.f );

	// CAUTION:
	// Normally you would only set the page size for scrollbar adjustments.
	// For demonstration purposes we do this for our scale widget too.
	// If page size isn't 0 a scale widget won't be able to be set to it's
	// maximum value. This is in fact also true for scrollbars, however
	// because they are used to scroll the page size must be subtracted from
	// the maximum.
	adjustment->SetPageSize( 20.f );

	// Additionally you can connect to the OnChange signal of an adjustment
	// to get notified when any of it's parameters are changed.
	adjustment->OnChange.Connect( &AdjustmentChange );

	// Just as with the entry widget we set custom requisitions for our
	// range widgets to make sure they don't look strange.
	scale->SetRequisition( sf::Vector2f( 80.f, 20.f ) );
	scrollbar->SetRequisition( sf::Vector2f( 0.f, 80.f ) );

	// To keep our scale's slider from expanding too much we use another box
	// set to verticle orientation.
	sfg::Box::Ptr scalebox = sfg::Box::Create( sfg::Box::Vertical );
	scalebox->Pack( scale, false, false );

	// Pack into box
	box->Pack( scalebox );
	box->Pack( scrollbar );
	box->Pack( label );

	// Set box spacing
	box->SetSpacing( 5.f );

	// Add our box to the window
	window->Add( box );
	window->SetBorderWidth( 10.f );

	// Start the game loop
	while ( app_window.IsOpened() ) {
		// Process events
		sf::Event event;

		while ( app_window.PollEvent( event ) ) {
			// Handle events
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

		// Draw the window
		window->Expose( app_window );

		// Update the window
		app_window.Display();
	}

	return EXIT_SUCCESS;
}

void AdjustmentChange() {
	std::stringstream sstr;
	sstr << adjustment->GetValue();
	label->SetText( sstr.str() );
}
