SET(SOURCES ui_ScreenMAIN240x320.c
    ui_ScreenMAIN240x135.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_shopping_cart_24dp_1f1f1f_fill0_wght400_grad0_opsz24_png.c
    ui_img_sledding_24dp_1f1f1f_fill0_wght400_grad0_opsz24_png.c
    ui_img_hotel_24dp_1f1f1f_fill0_wght400_grad0_opsz24_png.c)

add_library(ui ${SOURCES})
