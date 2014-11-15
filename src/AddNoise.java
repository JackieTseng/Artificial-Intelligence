import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import javax.swing.*;
import java.awt.EventQueue;
import java.awt.image.*;
import javax.imageio.*;
import java.lang.*;

public class AddNoise {
	public static void main(String[] args) {
		ImageProcess imgIO = new ImageProcess();
		for(int i = 10; i < 100; ++i) {
			int[][] temp = new int[384][256];
			temp = imgIO.readImage(String.valueOf(i) + ".png");
			temp = imgIO.quantize(temp, 384, 256, 2);
			imgIO.writeImage(temp, 384, 256, "Old" + String.valueOf(i) + ".png");
			for(int j = 0; j < 384; ++j) {
				for(int k = 0; k < 256; ++k) {
					if(Math.random() <= 0.1) {
						temp[j][k] = temp[j][k] == 0 ? 255 : 0;
					}
				}
			}
			imgIO.writeImage(temp, 384, 256, "New" + String.valueOf(i) + ".png");
			System.out.println("Noising picture " + String.valueOf(i));
		}
	}
}