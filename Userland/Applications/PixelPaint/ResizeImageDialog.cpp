/*
 * Copyright (c) 2022, Andrew Smith <andrew@alsmith.net>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ResizeImageDialog.h"
#include <Applications/PixelPaint/ResizeImageDialogGML.h>
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/CheckBox.h>
#include <LibGUI/ComboBox.h>
#include <LibGUI/Label.h>
#include <LibGUI/RadioButton.h>
#include <LibGUI/SpinBox.h>

namespace PixelPaint {

ResizeImageDialog::ResizeImageDialog(Gfx::IntSize const& suggested_size, GUI::Window* parent_window)
    : Dialog(parent_window)
{
    m_desired_size.set_width(max(1, suggested_size.width()));
    m_desired_size.set_height(max(1, suggested_size.height()));
    m_starting_aspect_ratio = m_desired_size.width() / static_cast<float>(m_desired_size.height());

    set_title("Resize Image");
    resize(260, 210);
    set_icon(parent_window->icon());

    auto& main_widget = set_main_widget<GUI::Widget>();
    if (!main_widget.load_from_gml(resize_image_dialog_gml))
        VERIFY_NOT_REACHED();

    auto width_spinbox = main_widget.find_descendant_of_type_named<GUI::SpinBox>("width_spinbox");
    auto height_spinbox = main_widget.find_descendant_of_type_named<GUI::SpinBox>("height_spinbox");
    auto keep_aspect_ratio_checkbox = main_widget.find_descendant_of_type_named<GUI::CheckBox>("keep_aspect_ratio_checkbox");

    VERIFY(width_spinbox);
    VERIFY(height_spinbox);
    VERIFY(keep_aspect_ratio_checkbox);

    width_spinbox->set_value(m_desired_size.width());
    width_spinbox->on_change = [this, height_spinbox, keep_aspect_ratio_checkbox](int value) {
        if (keep_aspect_ratio_checkbox->is_checked()) {
            int desired_height = static_cast<int>(roundf(value / m_starting_aspect_ratio));
            height_spinbox->set_value(desired_height, GUI::AllowCallback::No);
            m_desired_size.set_height(height_spinbox->value());
        }
        m_desired_size.set_width(value);
    };
    width_spinbox->on_return_pressed = [this]() {
        done(ExecResult::OK);
    };

    height_spinbox->set_value(m_desired_size.height());
    height_spinbox->on_change = [this, width_spinbox, keep_aspect_ratio_checkbox](int value) {
        if (keep_aspect_ratio_checkbox->is_checked()) {
            int desired_width = static_cast<int>(roundf(value * m_starting_aspect_ratio));
            width_spinbox->set_value(desired_width, GUI::AllowCallback::No);
            m_desired_size.set_width(width_spinbox->value());
        }
        m_desired_size.set_height(value);
    };
    height_spinbox->on_return_pressed = [this]() {
        done(ExecResult::OK);
    };

    keep_aspect_ratio_checkbox->on_checked = [this, height_spinbox](bool is_checked) {
        if (is_checked) {
            int desired_height = static_cast<int>(roundf(m_desired_size.width() / m_starting_aspect_ratio));
            height_spinbox->set_value(desired_height, GUI::AllowCallback::No);
            m_desired_size.set_height(height_spinbox->value());
        }
    };

    auto nearest_neighbor_radio = main_widget.find_descendant_of_type_named<GUI::RadioButton>("nearest_neighbor_radio");
    auto bilinear_radio = main_widget.find_descendant_of_type_named<GUI::RadioButton>("bilinear_radio");

    VERIFY(nearest_neighbor_radio);
    VERIFY(bilinear_radio);

    m_scaling_mode = Gfx::Painter::ScalingMode::NearestNeighbor;
    if (bilinear_radio->is_checked()) {
        m_scaling_mode = Gfx::Painter::ScalingMode::BilinearBlend;
    }

    nearest_neighbor_radio->on_checked = [this](bool is_checked) {
        if (is_checked)
            m_scaling_mode = Gfx::Painter::ScalingMode::NearestNeighbor;
    };
    bilinear_radio->on_checked = [this](bool is_checked) {
        if (is_checked)
            m_scaling_mode = Gfx::Painter::ScalingMode::BilinearBlend;
    };

    auto ok_button = main_widget.find_descendant_of_type_named<GUI::Button>("ok_button");
    auto cancel_button = main_widget.find_descendant_of_type_named<GUI::Button>("cancel_button");

    VERIFY(ok_button);
    VERIFY(cancel_button);

    ok_button->on_click = [this](auto) {
        done(ExecResult::OK);
    };

    cancel_button->on_click = [this](auto) {
        done(ExecResult::Cancel);
    };
}

}
