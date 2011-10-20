#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Object.hpp>
#include <SFGUI/Signal.hpp>
#include <SFGUI/DragInfo.hpp>
#include <SFGUI/SlotResult.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/any.hpp>
#include <map>

namespace sfg {

class Container;

/** Base class for widgets.
 */
class SFGUI_API Widget : public Object, public boost::enable_shared_from_this<Widget> {
	public:
		typedef boost::shared_ptr<Widget>  Ptr; //!< Shared pointer.
		typedef boost::shared_ptr<const Widget>  PtrConst; //!< Shared pointer.

		/** Widget state.
		 */
		enum State {
			Normal = 0, /*!< Normal. */
			Active, /*!< Active, e.g. when a button is pressed. */
			Prelight, /*!< Prelight, e.g. when the mouse moves over a widget. */
			Selected, /*!< Selected, e.g. when a list item in a list is selected. */
			Insensitive /*!< Insensitive, disabled widget. */
		};

		/** Event handling result.
		 */
		enum HandleEventResult {
			PassEvent = 0, ///< Pass event to children.
			EatEvent, ///< Stop processing the current event immediately.
			IgnoreEvent ///< Ignore event, but don't stop processing it in parent widget.
		};

		/** Destructor.
		 */
		virtual ~Widget();

		/** Check if widget is sensitive (enabled).
		 * @return true when sensitive.
		 */
		bool IsSensitive() const;

		/** Check if widget is visible, i.e. gets rendered.
		 * @return true when visible.
		 */
		bool IsVisible() const;

		/** Show (or hide) widget.
		 * @param show true to show, false to hide.
		 */
		void Show( bool show = true );

		/** Get name of widget.
		 * @return Name.
		 */
		virtual const std::string& GetName() const = 0;

		/** Grab focus.
		 */
		void GrabFocus();

		/** Allocate size.
		 * @param rect Rect.
		 */
		void AllocateSize( const sf::FloatRect& rect );

		/** Request new allocation at parent.
		 */
		void RequestSize();

		/** Get allocated size (position and size).
		 * @return Rect.
		 */
		const sf::FloatRect& GetAllocation() const;

		/** Get requested size (requisition).
		 * @return Size.
		 */
		const sf::Vector2f& GetRequisition() const;

		/** Set a custom requisition.
		 * This can be compared to setting a minimum widget size. Mostly setting it
		 * is not needed because sizers take care of proper widget dimensions. You
		 * can specify 0 for width or height to enable calculating the requisition
		 * for each axis. If you specify 0 both for width and height, normal
		 * requisition calculation is re-enabled.
		 * @param requisition Custom requisition (skip argument to disable custom requisition).
		 */
		void SetRequisition( const sf::Vector2f& requisition = sf::Vector2f( 0.f, 0.f ) );

		/** Set position.
		 * @param position Position.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Expose.
		 * Render widget to given target.
		 * @param target SFML render target.
		 */
		void Expose( sf::RenderTarget& target );

		/** Invalidate widget (prepare internal sf::Drawable).
		 * Implement InvalidateImpl() for your own code.
		 */
		void Invalidate();

		/** Set parent widget.
		 * Note that the parent must be a subclass of sfg::Container. You mostly
		 * don't want to call this method directly.
		 * @param parent Parent.
		 */
		void SetParent( Widget::Ptr parent );

		/** Get parent.
		 * @return Parent.
		 */
		boost::shared_ptr<Container> GetParent();

		/** Get parent.
		 * @return Parent.
		 */
		boost::shared_ptr<const Container> GetParent() const;

		/** Set widget's state.
		 * @param state State.
		 */
		void SetState( State state );

		/** Get widget's state.
		 * @return State.
		 */
		State GetState() const;

		/** Handle SFML event.
		 * Handle an SFML event and fire proper signals. Normally reimplemented by
		 * containers only.
		 * @return true when event has been processed (eaten).
		 */
		virtual HandleEventResult HandleEvent( const sf::Event& event );

		/** Get property.
		 * @param name Property name.
		 * @return Property value or T() if not set.
		 */
		template <typename T>
		const T& GetProperty( const std::string& name ) const;

		/** Set property.
		 * @param name Property name.
		 * @param value Value.
		 */
		template <typename T>
		void SetProperty( const std::string& name, const T& value );

		/** Check if widget has a property.
		 * @param property Name of property.
		 * @return true if exists.
		 */
		bool HasProperty( const std::string& property ) const;

		/** Get absolute position on screen.
		 * @return Absolute position.
		 */
		sf::Vector2f GetAbsolutePosition() const;

		/** Update position of drawable.
		 */
		virtual void UpdateDrawablePosition() const;

		/** Set ID.
		 * @param id ID.
		 */
		void SetId( const std::string& id );

		/** Get ID.
		 * @return ID or empty.
		 */
		const std::string& GetId() const;

		/** Set class.
		 * @param cls Class.
		 */
		void SetClass( const std::string& cls );

		/** Get class.
		 * @return Class or empty.
		 */
		const std::string& GetClass() const;

		// Signals.
		Signal<void( Ptr, State )>  OnStateChange; //!< Fired when state changed. (old state)
		Signal<void( Ptr )>  OnFocusChange; //!< Fired when focus grabbed or lost.

		Signal<void( Ptr, sf::RenderTarget& )>  OnExpose; //!< Fired when widget is being rendered.

		Signal<void( Ptr, const sf::FloatRect& )>  OnSizeAllocate; //!< Fired when widget's allocation changed.
		Signal<void( Ptr, const sf::FloatRect& )>  OnPositionChange; //!< Fired when widget's position changed.
		Signal<void( Ptr )> OnSizeRequest; //!< Fired when size was requested.

		Signal<void( Ptr, int, int )>  OnMouseEnter; //!< Fired when mouse entered widget. (x, y)
		Signal<bool( Ptr, int, int ), SlotResult>  OnMouseLeave; //!< Fired when mouse left widget. (x, y) Return true to keep the mouse move hook alive.
		Signal<void( Ptr, int, int )>  OnMouseMove; //!< Fired when mouse moved over widget. (x, y)
		Signal<bool( Ptr, int, int, sf::Mouse::Button ), SlotResult>  OnMouseButtonPress; //!< Fired when mouse button pressed. (x, y, button)
		Signal<bool( Ptr, int, int, sf::Mouse::Button ), SlotResult>  OnMouseButtonRelease; //!< Fired when mouse button released. (x, y, button)
		Signal<bool( Ptr, int, int, sf::Mouse::Button ), SlotResult>  OnMouseButtonClick; //!< Fired when mouse button clicked (pressed and released in same widget). (x, y, button)

		Signal<void( Ptr, const DragInfo& )>  OnDragStart; //!< Fired when dragging starts. (DragInfo)
		Signal<void( Ptr, const DragInfo& )>  OnDragMove; //!< Fired when dragged. (DragInfo)
		Signal<void( Ptr, const DragInfo& )>  OnDragEnd; //!< Fired when dragged. (DragInfo)

		Signal<void( Ptr, sf::Event::KeyEvent )> OnKeyPress; //!< Fired when a key is pressed while State == Active.
		Signal<void( Ptr, sf::Event::KeyEvent )> OnKeyRelease; //!< Fired when a key is released while State == Active.
		Signal<void( Ptr, sf::Uint32 Unicode )> OnText; //!< Fired when text is entered while State == Active.

	protected:
		/** Several flags for widgets.
		 */
		enum Flags {
			NoFlags = 0,
			Draggable = 1
		};

		/** Constructor.
		 */
		Widget();

		/** Invalidate implementation (redraw internally).
		 * Gets called whenever the widget needs to be redrawn, e.g. due to a call
		 * to Invalidate().
		 * @return Pointer to new drawable -- ownership is taken by caller.
		 */
		virtual sf::Drawable* InvalidateImpl();

		/** Requisition implementation (recalculate requisition).
		 * @return New requisition.
		 */
		virtual sf::Vector2f GetRequisitionImpl() const = 0;

		/** Check if mouse is inside widget.
		 * @return true if mouse is inside.
		 */
		bool IsMouseInWidget() const;

		/** Set widget flags.
		 * @param flags Flags.
		 */
		void SetFlags( int flags );

		/** Check if flag set.
		 * @return true when set.
		 */
		bool HasFlag( Flags flag ) const;

	private:
		typedef std::map<const std::string, boost::any> PropertiesMap;

		void GrabFocus( Ptr widget );

		boost::weak_ptr<Container>  m_parent;

		std::string m_id;
		std::string m_class;

		bool  m_sensitive;
		bool  m_visible;
		Ptr   m_focus_widget;

		State  m_state;
		bool  m_mouse_in;
		int  m_mouse_button_down;

		sf::FloatRect  m_allocation;
		mutable sf::Vector2f   m_requisition;
		boost::scoped_ptr<sf::Vector2f> m_custom_requisition;

		bool  m_invalidated;
		mutable bool  m_recalc_requisition;

		int  m_flags;
		boost::scoped_ptr<DragInfo>  m_drag_info;

		PropertiesMap  m_properties;

		mutable boost::scoped_ptr<sf::Drawable>  m_drawable;
};

}

#include "Widget.inl"
