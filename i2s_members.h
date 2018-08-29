MEMBER(reserved_0)
MEMBER(   reserved_4)
MEMBER(        conf.tx_reset)//       1)//
MEMBER(        conf.rx_reset)//       1)//
MEMBER(        conf.tx_fifo_reset)//  1)//
MEMBER(        conf.rx_fifo_reset)//  1)//
MEMBER(        conf.tx_start)//       1)//
MEMBER(        conf.rx_start)//       1)//
MEMBER(        conf.tx_slave_mod)//   1)//
MEMBER(        conf.rx_slave_mod)//   1)//
MEMBER(        conf.tx_right_first)// 1)//
MEMBER(        conf.rx_right_first)// 1)//
MEMBER(        conf.tx_msb_shift)//   1)//
MEMBER(        conf.rx_msb_shift)//   1)//
MEMBER(        conf.tx_short_sync)//  1)//
MEMBER(        conf.rx_short_sync)//  1)//
MEMBER(        conf.tx_mono)//        1)//
MEMBER(        conf.rx_mono)//        1)//
MEMBER(        conf.tx_msb_right)//   1)//
MEMBER(        conf.rx_msb_right)//   1)//
MEMBER(        conf.sig_loopback)//   1)//
MEMBER(        conf.reserved19)//    13)//
MEMBER(        int_raw.rx_take_data)//  1)//
MEMBER(        int_raw.tx_put_data)//   1)//
MEMBER(        int_raw.rx_wfull)//      1)//
MEMBER(        int_raw.rx_rempty)//     1)//
MEMBER(        int_raw.tx_wfull)//      1)//
MEMBER(        int_raw.tx_rempty)//     1)//
MEMBER(        int_raw.rx_hung)//       1)//
MEMBER(        int_raw.tx_hung)//       1)//
MEMBER(        int_raw.in_done)//       1)//
MEMBER(        int_raw.in_suc_eof)//    1)//
MEMBER(        int_raw.in_err_eof)//    1)//
MEMBER(        int_raw.out_done)//      1)//
MEMBER(        int_raw.out_eof)//       1)//
MEMBER(        int_raw.in_dscr_err)//   1)//
MEMBER(        int_raw.out_dscr_err)//  1)//
MEMBER(        int_raw.in_dscr_empty)// 1)//
MEMBER(        int_raw.out_total_eof)// 1)//
MEMBER(        int_raw.reserved17)//   15)//
MEMBER(        int_ena.rx_take_data)//  1)//
MEMBER(        int_ena.tx_put_data)//   1)//
MEMBER(        int_ena.rx_wfull)//      1)//
MEMBER(        int_ena.rx_rempty)//     1)//
MEMBER(        int_ena.tx_wfull)//      1)//
MEMBER(        int_ena.tx_rempty)//     1)//
MEMBER(        int_ena.rx_hung)//       1)//
MEMBER(        int_ena.tx_hung)//       1)//
MEMBER(        int_ena.in_done)//       1)//
MEMBER(        int_ena.in_suc_eof)//    1)//
MEMBER(        int_ena.in_err_eof)//    1)//
MEMBER(        int_ena.out_done)//      1)//
MEMBER(        int_ena.out_eof)//       1)//
MEMBER(        int_ena.in_dscr_err)//   1)//
MEMBER(        int_ena.out_dscr_err)//  1)//
MEMBER(        int_ena.in_dscr_empty)// 1)//
MEMBER(        int_ena.out_total_eof)// 1)//
MEMBER(        int_ena.reserved17)//   15)//
MEMBER(        timing.tx_bck_in_delay)//   2)//
MEMBER(        timing.tx_ws_in_delay)//    2)//
MEMBER(        timing.rx_bck_in_delay)//   2)//
MEMBER(        timing.rx_ws_in_delay)//    2)//
MEMBER(        timing.rx_sd_in_delay)//    2)//
MEMBER(        timing.tx_bck_out_delay)//  2)//
MEMBER(        timing.tx_ws_out_delay)//   2)//
MEMBER(        timing.tx_sd_out_delay)//   2)//
MEMBER(        timing.rx_ws_out_delay)//   2)//
MEMBER(        timing.rx_bck_out_delay)//  2)//
MEMBER(        timing.tx_dsync_sw)//       1)//
MEMBER(        timing.rx_dsync_sw)//       1)//
MEMBER(        timing.data_enable_delay)// 2)//
MEMBER(        timing.tx_bck_in_inv)//     1)//
MEMBER(        timing.reserved25)//        7)//
MEMBER(        fifo_conf.rx_data_num)//          6)//
MEMBER(        fifo_conf.tx_data_num)//          6)//
MEMBER(        fifo_conf.dscr_en)//              1)//
MEMBER(        fifo_conf.tx_fifo_mod)//          3)//
MEMBER(        fifo_conf.rx_fifo_mod)//          3)//
MEMBER(        fifo_conf.tx_fifo_mod_force_en)// 1)//
MEMBER(        fifo_conf.rx_fifo_mod_force_en)// 1)//
MEMBER(        fifo_conf.reserved21)//          11)//
MEMBER(rx_eof_num)//
MEMBER(conf_single_data)//
MEMBER(        conf_chan.tx_chan_mod)// 3)//
MEMBER(        conf_chan.rx_chan_mod)// 2)//
MEMBER(        conf_chan.reserved5)//  27)//
MEMBER(        out_link.addr)//       20)//
MEMBER(        out_link.reserved20)//  8)//
MEMBER(        out_link.stop)//        1)//
MEMBER(        out_link.start)//       1)//
MEMBER(        out_link.restart)//     1)//
MEMBER(        out_link.park)//        1)//
MEMBER(        in_link.addr)//       20)//
MEMBER(        in_link.reserved20)//  8)//
MEMBER(        in_link.stop)//        1)//
MEMBER(        in_link.start)//       1)//
MEMBER(        in_link.restart)//     1)//
MEMBER(        in_link.park)//        1)//
MEMBER(   out_eof_des_addr)//
MEMBER(   in_eof_des_addr)//
MEMBER(   out_eof_bfr_des_addr)//
MEMBER(        ahb_test.mode)//       3)//
MEMBER(        ahb_test.reserved3)//  1)//
MEMBER(        ahb_test.addr)//       2)//
MEMBER(        ahb_test.reserved6)// 26)//
MEMBER(in_link_dscr)//
MEMBER(in_link_dscr_bf0)//
MEMBER(in_link_dscr_bf1)//
MEMBER(out_link_dscr)//
MEMBER(out_link_dscr_bf0)//
MEMBER(out_link_dscr_bf1)//
MEMBER(        lc_conf.in_rst)//             1)//
MEMBER(        lc_conf.out_rst)//            1)//
MEMBER(        lc_conf.ahbm_fifo_rst)//      1)//
MEMBER(        lc_conf.ahbm_rst)//           1)//
MEMBER(        lc_conf.out_loop_test)//      1)//
MEMBER(        lc_conf.in_loop_test)//       1)//
MEMBER(        lc_conf.out_auto_wrback)//    1)//
MEMBER(        lc_conf.out_no_restart_clr)// 1)//
MEMBER(        lc_conf.out_eof_mode)//       1)//
MEMBER(        lc_conf.outdscr_burst_en)//   1)//
MEMBER(        lc_conf.indscr_burst_en)//    1)//
MEMBER(        lc_conf.out_data_burst_en)//  1)//
MEMBER(        lc_conf.check_owner)//        1)//
MEMBER(        lc_conf.mem_trans_en)//       1)//
MEMBER(        lc_conf.reserved14)//        18)//
MEMBER(        out_fifo_push.wdata)//      9)//
MEMBER(        out_fifo_push.reserved9)//  7)//
MEMBER(        out_fifo_push.push)//       1)//
MEMBER(        out_fifo_push.reserved17)// 15)//
MEMBER(        in_fifo_pop.rdata)//      12)//
MEMBER(        in_fifo_pop.reserved12)//  4)//
MEMBER(        in_fifo_pop.pop)//         1)//
MEMBER(        in_fifo_pop.reserved17)// 15)//
MEMBER(lc_state0)//
MEMBER(lc_state1)//
MEMBER(        lc_hung_conf.fifo_timeout)//       8)//
MEMBER(        lc_hung_conf.fifo_timeout_shift)// 3)//
MEMBER(        lc_hung_conf.fifo_timeout_ena)//   1)//
MEMBER(        lc_hung_conf.reserved12)//        20)//
MEMBER(reserved_78)//
MEMBER(reserved_7c)//
MEMBER(        cvsd_conf0.y_max)//16)//
MEMBER(        cvsd_conf0.y_min)//16)//
MEMBER(        cvsd_conf1.sigma_max)//16)//
MEMBER(        cvsd_conf1.sigma_min)//16)//
MEMBER(        cvsd_conf2.cvsd_k)//     3)//
MEMBER(        cvsd_conf2.cvsd_j)//     3)//
MEMBER(        cvsd_conf2.cvsd_beta)// 10)//
MEMBER(        cvsd_conf2.cvsd_h)//     3)//
MEMBER(        cvsd_conf2.reserved19)//13)//
MEMBER(        plc_conf0.good_pack_max)//    6)//
MEMBER(        plc_conf0.n_err_seg)//        3)//
MEMBER(        plc_conf0.shift_rate)//       3)//
MEMBER(        plc_conf0.max_slide_sample)// 8)//
MEMBER(        plc_conf0.pack_len_8k)//      5)//
MEMBER(        plc_conf0.n_min_err)//        3)//
MEMBER(        plc_conf0.reserved28)//       4)//
MEMBER(        plc_conf1.bad_cef_atten_para)//       8)//
MEMBER(        plc_conf1.bad_cef_atten_para_shift)// 4)//
MEMBER(        plc_conf1.bad_ola_win2_para_shift)//  4)//
MEMBER(        plc_conf1.bad_ola_win2_para)//        8)//
MEMBER(        plc_conf1.slide_win_len)//            8)//
MEMBER(        plc_conf2.cvsd_seg_mod)// 2)//
MEMBER(        plc_conf2.min_period)//   5)//
MEMBER(        plc_conf2.reserved7)//   25)//
MEMBER(        esco_conf0.en)//                1)//
MEMBER(        esco_conf0.chan_mod)//          1)//
MEMBER(        esco_conf0.cvsd_dec_pack_err)// 1)//
MEMBER(        esco_conf0.cvsd_pack_len_8k)//  5)//
MEMBER(        esco_conf0.cvsd_inf_en)//       1)//
MEMBER(        esco_conf0.cvsd_dec_start)//    1)//
MEMBER(        esco_conf0.cvsd_dec_reset)//    1)//
MEMBER(        esco_conf0.plc_en)//            1)//
MEMBER(        esco_conf0.plc2dma_en)//        1)//
MEMBER(        esco_conf0.reserved13)//       19)//
MEMBER(        sco_conf0.with_en)//        1)//
MEMBER(        sco_conf0.no_en)//          1)//
MEMBER(        sco_conf0.cvsd_enc_start)// 1)//
MEMBER(        sco_conf0.cvsd_enc_reset)// 1)//
MEMBER(        sco_conf0.reserved4)//     28)//
MEMBER(        conf1.tx_pcm_conf)//    3)//
MEMBER(        conf1.tx_pcm_bypass)//  1)//
MEMBER(        conf1.rx_pcm_conf)//    3)//
MEMBER(        conf1.rx_pcm_bypass)//  1)//
MEMBER(        conf1.tx_stop_en)//     1)//
MEMBER(        conf1.tx_zeros_rm_en)// 1)//
MEMBER(        conf1.reserved10)//    22)//
MEMBER(        pd_conf.fifo_force_pd)//    1)//
MEMBER(        pd_conf.fifo_force_pu)//    1)//
MEMBER(        pd_conf.plc_mem_force_pd)// 1)//
MEMBER(        pd_conf.plc_mem_force_pu)// 1)//
MEMBER(        pd_conf.reserved4)//       28)//
MEMBER(        conf2.camera_en)//           1)//
MEMBER(        conf2.lcd_tx_wrx2_en)//      1)//
MEMBER(        conf2.lcd_tx_sdx2_en)//      1)//
MEMBER(        conf2.data_enable_test_en)// 1)//
MEMBER(        conf2.data_enable)//         1)//
MEMBER(        conf2.lcd_en)//              1)//
MEMBER(        conf2.ext_adc_start_en)//    1)//
MEMBER(        conf2.inter_valid_en)//      1)//
MEMBER(        conf2.reserved8)//          24)//
MEMBER(        clkm_conf.clkm_div_num)// 8)//
MEMBER(        clkm_conf.clkm_div_b)//   6)//
MEMBER(        clkm_conf.clkm_div_a)//   6)//
MEMBER(        clkm_conf.clk_en)//       1)//
MEMBER(        clkm_conf.clka_en)//      1)//
MEMBER(        clkm_conf.reserved22)//  10)//
MEMBER(        sample_rate_conf.tx_bck_div_num)// 6)//
MEMBER(        sample_rate_conf.rx_bck_div_num)// 6)//
MEMBER(        sample_rate_conf.tx_bits_mod)//    6)//
MEMBER(        sample_rate_conf.rx_bits_mod)//    6)//
MEMBER(        sample_rate_conf.reserved24)//     8)//
MEMBER(        pdm_conf.tx_pdm_en)//              1)//
MEMBER(        pdm_conf.rx_pdm_en)//              1)//
MEMBER(        pdm_conf.pcm2pdm_conv_en)//        1)//
MEMBER(        pdm_conf.pdm2pcm_conv_en)//        1)//
MEMBER(        pdm_conf.tx_sinc_osr2)//           4)//
MEMBER(        pdm_conf.tx_prescale)//            8)//
MEMBER(        pdm_conf.tx_hp_in_shift)//         2)//
MEMBER(        pdm_conf.tx_lp_in_shift)//         2)//
MEMBER(        pdm_conf.tx_sinc_in_shift)//       2)//
MEMBER(        pdm_conf.tx_sigmadelta_in_shift)// 2)//
MEMBER(        pdm_conf.rx_sinc_dsr_16_en)//      1)//
MEMBER(        pdm_conf.txhp_bypass)//            1)//
MEMBER(        pdm_conf.reserved26)//             6)//
MEMBER(        pdm_freq_conf.tx_pdm_fs)// 10)//
MEMBER(        pdm_freq_conf.tx_pdm_fp)// 10)//
MEMBER(        pdm_freq_conf.reserved20)//12)//
MEMBER(        state.tx_idle)//            1)//
MEMBER(        state.tx_fifo_reset_back)// 1)//
MEMBER(        state.rx_fifo_reset_back)// 1)//
MEMBER(        state.reserved3)//         29)//

