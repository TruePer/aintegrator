import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import Qt.labs.settings 1.0
import Material 0.2


Item {
	property ApplicationWindow window

	// Default properties for the application's first run.
	property int defaultX: 100
	property int defaultY: 100
	property int defaultWidth: 500
	property int defaultHeight: 500
	property bool defaultMaximised: false

	Settings {
		id: windowStateSettings
		category: "WindowState"
		property int x
		property int y
		property int width
		property int height
		property bool maximised
		property string primaryColor
		property string accentColor
		property string backgroundColor
		property string tabHighlightColor
	}

	Component.onCompleted: {
		if (windowStateSettings.width === 0 || windowStateSettings.height === 0) {
			// First run, or width/height are screwed up.
			curX                           = defaultX;
			curY                           = defaultY;
			curWidth                       = defaultWidth;
			curHeight                      = defaultHeight;
			curMaximised                   = defaultMaximised
			window.theme.primaryColor      = "blue"
			window.theme.accentColor       = "red"
			window.theme.backgroundColor   = "white"
			window.theme.tabHighlightColor = Theme.isDarkColor(Theme.primaryColor) ? "white" : "black"
		}
		else {
			curX = windowStateSettings.x;
			curY = windowStateSettings.y;
			curWidth = windowStateSettings.width;
			curHeight = windowStateSettings.height;
			curMaximised = windowStateSettings.maximised
			window.theme.primaryColor      = windowStateSettings.primaryColor
			window.theme.accentColor       = windowStateSettings.accentColor
			window.theme.backgroundColor   = windowStateSettings.backgroundColor
			window.theme.tabHighlightColor = windowStateSettings.tabHighlightColor
		}
		window.x = prevX = curX;
		window.y = prevY = curY;
		window.width = prevWidth = curWidth;
		window.height = prevHeight = curHeight;

		if (curMaximised)
			window.visibility = Window.Maximized;
	}

	// Remember the windowed geometry, and whether it is maximised or not.
	// Internal use only.
	property int curX
	property int curY
	property int curWidth
	property int curHeight
	property bool curMaximised

	// We also have to save the previous values of X/Y/Width/Height so they can be restored if we maximise, since we
	// can't tell that the updated X,Y values are because of maximisation until *after* the maximisation.
	property int prevX
	property int prevY
	property int prevWidth
	property int prevHeight

	Connections {
		target: window
		onVisibilityChanged: {
			if (window.visibility === Window.Maximized)
			{
				curMaximised = true;
				// Ignore the latest X/Y/width/height values.
				curX = prevX;
				curY = prevY;
				curWidth = prevWidth;
				curHeight = prevHeight;
			}
			else if (window.visibility === Window.Windowed)
			{
				curMaximised = false;
			}
			else if (window.visibility === Window.Hidden)
			{
				// Save settings.
				windowStateSettings.x = curX;
				windowStateSettings.y = curY;
				windowStateSettings.width = curWidth;
				windowStateSettings.height = curHeight;
				windowStateSettings.maximised = curMaximised;
				windowStateSettings.primaryColor = window.theme.primaryColor
				windowStateSettings.accentColor = window.theme.accentColor
				windowStateSettings.backgroundColor = window.theme.backgroundColor
				windowStateSettings.tabHighlightColor = window.theme.tabHighlightColor
			}
		}

		// We can't use window.visibility here to ignore the maximised geometry because it changes after the geometry.
		// Instead we cache the two previous values and revert them if maximised.
		onXChanged: {
			prevX = curX;
			curX = window.x;
		}
		onYChanged: {
			prevY = curY;
			curY = window.y;
		}
		onWidthChanged: {
			prevWidth = curWidth;
			curWidth = window.width;
		}
		onHeightChanged: {
			prevHeight = curHeight;
			curHeight = window.height;
		}
	}

}
