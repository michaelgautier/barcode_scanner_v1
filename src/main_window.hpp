#ifndef inc_gautier_ui_main_window
#define inc_gautier_ui_main_window

#include <gtkmm.h>
#include "barcode_record.hpp"

class main_window : public Gtk::ApplicationWindow {
	public:
		main_window();
	protected:
	        //Contains all
	        Gtk::Paned top_frame;

                //Input
                Gtk::Box input_frame; //Contains the input fields
	        Gtk::Label barcode_field_label;
	        Gtk::Entry barcode_field;
	        
                //List
                Gtk::ScrolledWindow barcodes_scroller;
                Gtk::ColumnView barcodes;
                
                //List model
                Glib::RefPtr<Gio::ListStore<barcode_record>> barcode_list;
                Glib::RefPtr<Gtk::ColumnViewColumn> barcode_column;

                Glib::RefPtr<barcode_record> barcode_row;
                Glib::RefPtr<Gtk::ListItem> barcode_entry;
                Glib::RefPtr<Gtk::SignalListItemFactory> barcode_list_signal_factory;
                Glib::RefPtr<Gtk::Label> barcode_label;

                void on_barcode_bind(const Glib::RefPtr<Gtk::ListItem>& list_item);
                void on_barcode_activate();
};
#endif
