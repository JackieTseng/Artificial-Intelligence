import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import javax.swing.*;
import java.awt.EventQueue;
import java.awt.image.*;
import javax.imageio.*;
import java.lang.*;

public class ImageProcess {
	public static int[][] readImage(String name) {
        File filePath = new File(name);
        try {
            BufferedImage imgBuffer = ImageIO.read(filePath);
            int height = imgBuffer.getHeight();
            int width = imgBuffer.getWidth();
            int[][] inputImage = new int[width][height];
            int rgb, gray;
            for(int i = 0; i < width; ++i) {
                for(int j = 0; j < height; ++j) {
                    rgb = imgBuffer.getRGB(i, j) & 0xffffff;
                    gray = (int)(((rgb & 0x00ff0000) >> 16) * 0.299 + ((rgb & 0x0000ff00) >> 8) * 0.587 + (rgb & 0x000000ff) * 0.114);
                    inputImage[i][j] = gray;
                }
            }
            return inputImage;
        }
        catch (IOException err) {
            err.printStackTrace();
        }
        return new int[1][1];
    }
    public static int[][] quantize(int[][] input_img, int row, int col, int level) {
        level = level < 2 ? 2 : level;
        /* calculate the interval */
        int interval = 255 / (level - 1);
        int pos;
        int[][] temp = new int[row][col];
        float average;
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                pos = input_img[i][j] / interval;
                /* decide the gray level */
                average = (float)(((pos << 1) + 1) * interval) / (float)2.0;
                if((float)input_img[i][j] < average) {
                    temp[i][j] = pos * interval;
                }
                else {
                    temp[i][j] = (pos + 1) * interval;
                }
            }
        }
        return temp;
    }
    public static void writeImage(int[][] input_img, int row, int col, String name)  {
        try {
            BufferedImage outFile = new BufferedImage(row, col, BufferedImage.TYPE_INT_RGB);
            int temp;
            for(int i = 0; i < row; ++i) {
                for (int j = 0; j < col; ++j) {
                    temp = ((255 & 0xff) << 24)
                           | (((int)input_img[i][j]  & 0xff) << 16)
                           | (((int)input_img[i][j] & 0xff) << 8)
                           | ((int)input_img[i][j]  & 0xff);
                    outFile.setRGB(i, j, temp);
                }
            }
            ImageIO.write(outFile, "png", new File(name));
         }
         catch (IOException err) {
            err.printStackTrace();
        }
    }
}