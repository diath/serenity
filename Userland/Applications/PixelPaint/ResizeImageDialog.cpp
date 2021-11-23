/*
 * Copyright (c) 2021, Kamil Chojnowski <contact@diath.net>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ResizeImageDialog.h"
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/Label.h>
#include <LibGUI/SpinBox.h>
#include <LibGUI/TextBox.h>

namespace PixelPaint {

ResizeImageDialog::ResizeImageDialog(Gfx::IntSize const& current_size, GUI::Window* parent_window)
    : Dialog(parent_window)
{
    set_title("Resize image");
    set_icon(parent_window->icon());
    resize(200, 160);

    auto& main_widget = set_main_widget<GUI::Widget>();
    main_widget.set_fill_with_background_color(true);

    auto& layout = main_widget.set_layout<GUI::VerticalBoxLayout>();
    layout.set_margins(4);

    auto& width_label = main_widget.add<GUI::Label>("Width:");
    width_label.set_text_alignment(Gfx::TextAlignment::CenterLeft);

    auto& width_spinbox = main_widget.add<GUI::SpinBox>();

    auto& height_label = main_widget.add<GUI::Label>("Height:");
    height_label.set_text_alignment(Gfx::TextAlignment::CenterLeft);

    auto& height_spinbox = main_widget.add<GUI::SpinBox>();

    auto& button_container = main_widget.add<GUI::Widget>();
    button_container.set_layout<GUI::HorizontalBoxLayout>();

    auto& ok_button = button_container.add<GUI::Button>("OK");
    ok_button.on_click = [this](auto) {
        done(ExecOK);
    };

    auto& cancel_button = button_container.add<GUI::Button>("Cancel");
    cancel_button.on_click = [this](auto) {
        done(ExecCancel);
    };

    width_spinbox.on_change = [this](int value) {
        m_image_size.set_width(value);
    };

    height_spinbox.on_change = [this](int value) {
        m_image_size.set_height(value);
    };

    width_spinbox.set_range(1, 16384);
    height_spinbox.set_range(1, 16384);

    width_spinbox.set_value(current_size.width());
    height_spinbox.set_value(current_size.height());
}

}
