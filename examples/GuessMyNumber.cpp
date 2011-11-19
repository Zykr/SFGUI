#include <SFGUI/Window.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <sstream>
#include <cstdlib>

class GuessMyNumber {
	public:
		GuessMyNumber();

		void Run();

	private:
		static const std::string TITLE;

		void ResetGame();
		void UpdateUI();

		void OnGuessClick();
		void OnNewGameClick();

		uint8_t m_number;
		uint32_t m_tries;

		sfg::Entry::Ptr m_number_entry;
		sfg::Label::Ptr m_tries_label;
		sfg::Label::Ptr m_hint_label;
		sfg::Button::Ptr m_guess_button;
};

int main() {
	GuessMyNumber game;
	game.Run();

	return 0;
}

const std::string GuessMyNumber::TITLE = "Guess My Number (SFGUI)";

GuessMyNumber::GuessMyNumber() :
	m_number_entry( sfg::Entry::Create() ),
	m_tries_label( sfg::Label::Create() ),
	m_hint_label( sfg::Label::Create() ),
	m_guess_button( sfg::Button::Create( "Guess" ) )
{
}

void GuessMyNumber::ResetGame() {
	m_tries = 0;
	m_number = static_cast<uint8_t>( std::rand() % 100 + 1 );

	m_hint_label->SetText( "-" );

	UpdateUI();
	m_guess_button->Show( true );
}

void GuessMyNumber::UpdateUI() {
	std::stringstream sstr;
	sstr << m_tries;
	m_tries_label->SetText( sstr.str() );
}

void GuessMyNumber::OnNewGameClick() {
	ResetGame();
}

void GuessMyNumber::OnGuessClick() {
	// Validate number.
	uint32_t buf_number( 0 );

	std::stringstream sstr( static_cast<std::string>( m_number_entry->GetText() ) );
	sstr >> buf_number;

	if( buf_number < 1 || buf_number > 100 ) {
		m_hint_label->SetText( "Enter a number from 1 to 100." );
		return;
	}

	++m_tries;
	UpdateUI();

	uint8_t number( static_cast<uint8_t>( buf_number ) );
	if( number < m_number ) {
		m_hint_label->SetText( "My number is higher." );
	}
	else if( number > m_number ) {
		m_hint_label->SetText( "My number is lower." );
	}
	else {
		m_hint_label->SetText( "Correct!" );
		m_guess_button->Show( false );
	}

	m_number_entry->SetText( "" );
}

void GuessMyNumber::Run() {
	sf::RenderWindow render_window( sf::VideoMode( 1024, 768, 32 ), TITLE );
	sf::Event event;

	// Create widgets.
	sfg::Window::Ptr window( sfg::Window::Create() );
	window->SetTitle( TITLE );

	sfg::Button::Ptr new_game_button( sfg::Button::Create( "New game" ) );
	new_game_button->OnClick.Connect( &GuessMyNumber::OnNewGameClick, this );

	m_guess_button->SetId( "guess" );
	m_guess_button->OnClick.Connect( &GuessMyNumber::OnGuessClick, this );

	// Layout.
	sfg::Table::Ptr table( sfg::Table::Create() );
	table->Attach( sfg::Label::Create( "Your guess:" ), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( m_number_entry, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ) );
	table->Attach( sfg::Label::Create( "Tries:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( m_tries_label, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ) );
	table->Attach( sfg::Label::Create( "Hint:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	table->Attach( m_hint_label, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ) );

	table->SetColumnSpacings( 5.f );
	table->SetRowSpacings( 5.f );

	sfg::Box::Ptr buttons_box( sfg::Box::Create( sfg::Box::Horizontal, 5.f ) );
	buttons_box->Pack( new_game_button );
	buttons_box->Pack( m_guess_button );

	sfg::Box::Ptr content_vbox( sfg::Box::Create( sfg::Box::Vertical, 5.f ) );
	content_vbox->SetBorderWidth( 10.f );
	content_vbox->Pack( sfg::Label::Create( "Guess my number, it's from 1 to 100!" ) );
	content_vbox->Pack( table );
	content_vbox->Pack( buttons_box );

	window->Add( content_vbox );

	ResetGame();

	window->SetPosition(
		sf::Vector2f(
			static_cast<float>( render_window.GetWidth() / 2 ) - window->GetAllocation().Width / 2.f,
			static_cast<float>( render_window.GetHeight() / 2 ) - window->GetAllocation().Height / 2.f
		)
	);

	// Custom properties.
	sfg::Context::Get().GetEngine().SetProperty( "Button#guess", "BackgroundColor", sf::Color( 0, 100, 0 ) );
	sfg::Context::Get().GetEngine().SetProperty( "Button#guess", "BorderColor", sf::Color( 0, 100, 0 ) );
	sfg::Context::Get().GetEngine().SetProperty( "Button#guess:Prelight", "BackgroundColor", sf::Color( 0, 130, 0 ) );
	sfg::Context::Get().GetEngine().SetProperty( "Button#guess:Prelight", "BorderColor", sf::Color( 0, 130, 0 ) );
	sfg::Context::Get().GetEngine().SetProperty( "Button#guess > Label", "FontSize", 20.f );

	// Make sure all properties are applied.
	window->Refresh();

	while( render_window.IsOpened() ) {
		while( render_window.PollEvent( event ) ) {
			if(
				(event.Type == sf::Event::Closed) ||
				(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::Escape)
			) {
				render_window.Close();
			}

			window->HandleEvent( event );
		}

		render_window.Clear();
		window->Expose( render_window );
		render_window.Display();
	}
}
