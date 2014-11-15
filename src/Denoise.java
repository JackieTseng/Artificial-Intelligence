import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import javax.swing.*;
import java.awt.EventQueue;
import java.awt.image.*;
import javax.imageio.*;
import java.lang.*;

public class Denoise {
	public static void main(String[] args) {
        for (int i = 10; i < 100; ++i) {
            if (args.length == 0 || (args.length == 1 && args[0].equals("gre"))) {
                imageDenosingByGreedy(0.8, 3.8, 8.4, "Noised" + String.valueOf(i), "Original" + String.valueOf(i));
            } else if (args.length == 1 && args[0].equals("sa")) {
                imageDenosingBySA(0.8, 3.8, 8.4, "Noised" + String.valueOf(i), "Original" + String.valueOf(i));
            }
        }
        System.out.println(result / 90.0);
	}

    /**
     * Image Denosing Method
     * @param h, b, n  parameters of the expression
     * @param nPic name of noised image
     * @param oPic name of original image
     */
    public static void imageDenosingByGreedy(double h, double b, double n, String nPic, String oPic) {
        int[][] y, x, origin;
        origin = readImage(oPic + ".png");
        y = readImage(nPic + ".png");
        int HEIGHT = origin.length;
        int WIDTH = origin[0].length;
        int TOTAL = HEIGHT * WIDTH;
        x = new int[HEIGHT][WIDTH];

        for(int i = 0; i < HEIGHT; ++i) {
            for(int j = 0; j < WIDTH; ++j) {
                x[i][j] = y[i][j] = (y[i][j] == 255) ? -1 : 1;
            }
        }
        double minEnergy = calculateEnergy(h, b, n, x, y);
        double curEnergy, temp;
        curEnergy = minEnergy;
        int pos = 0, row, col, sum = 0, xx, yy, times = 0;
        while(true) {
            row = pos / WIDTH;
            col = pos % WIDTH;
            temp = curEnergy;
            x[row][col] *= -1;
            curEnergy += 2.0 * h * (double)x[row][col];
            curEnergy -= 2.0 * n * (double)y[row][col] * (double)x[row][col];
            sum = 0;
            for(int i = 0; i < 4; ++i) {
                xx = row + adjacent4[i][0];
                yy = col + adjacent4[i][1];
                if(xx >= 0 && xx < HEIGHT && yy >= 0 && yy < WIDTH) {
                    sum += x[row][col] * x[xx][yy];
                }
            }
            curEnergy -= 4.0 * b * (double)sum;
            if(curEnergy >= temp) {
                x[row][col] *= -1;
                curEnergy = temp;
            }
            ++pos;
            if (pos == HEIGHT * WIDTH) {
                if(times == 1) {
                    break;
                }
                else {
                   pos = 0;
                   ++times;
                }
            }
        }

        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                x[i][j] = (x[i][j] == -1) ? 255 : 0;
            }
        }
        writeImage(x, oPic + "Denoised.png");
        compareImage(origin, x);
    }

    /**
     * Image Denosing Method
     * @param h, b, n  parameters of the expression
     * @param nPic name of noised image
     * @param oPic name of original image
     */
    public static void imageDenosingBySA(double h, double b, double n, String nPic, String oPic) {
        int[][] y, x, origin;
        origin = readImage(oPic + ".png");
        x = readImage(nPic + ".png");
        int HEIGHT = origin.length;
        int WIDTH = origin[0].length;
        y = new int[HEIGHT][WIDTH];

        // Transform : White : 1, Black : -1
        for (int j = 0; j < HEIGHT; ++j) {
            for (int k = 0; k < WIDTH; ++k) {
                y[j][k] = x[j][k] = (x[j][k] == 255) ? -1 : 1;
            }
        }
        
        double resultEnergy = calculateEnergy(h, b, n, x, y);
        double curEnergy, newEnergy = 0;
        int roundCounter = 0;
        
        /**
         * Simulated Annealing Algorithm
         * @param curT the current temperature
         * @param minT the final temperature
         * @param dE   the energy distance between two states
         * @param r    the reduce rate
         */
        double curT = 100000, minT = 0.01, dE = 0, r = 0.95;
        Random random = new Random(System.currentTimeMillis());
        while (curT > minT) {
            roundCounter++;
            for (int j = 0; j < HEIGHT; ++j) {
                for (int k = 0; k < WIDTH; ++k) {
                    curEnergy = calculatePartEnergy(x, y, j, k, 1, h, b, n); // Y(i)
                    x[j][k] = -y[j][k]; // Change the state for calculating
                    newEnergy = calculatePartEnergy(x, y, j, k, 1, h, b, n); // Y(i+1)
                    dE = curEnergy - newEnergy;
                    if (dE <= 0) {
                        x[j][k] = -x[j][k]; // Always accept if better
                    } else {
                        if (Math.exp((-dE)/curT) > random.nextDouble()) {
                            y[j][k] = -y[j][k]; // Maybe accept though worse
                        }
                    }       
                }
            }
            curT *= r; // Temperture decrease
            newEnergy = calculateEnergy(h, b, n, x, y);
            if ((resultEnergy - newEnergy) > Math.pow(10, -10)) {
                resultEnergy = newEnergy;
            }
        }
        // Transform : White : 0, Black : 255 
        for (int j = 0; j < HEIGHT; ++j) {
            for (int k = 0; k < WIDTH; ++k) {
                x[j][k] = (x[j][k] == 1) ? 0 : 255;
            }
        }
        writeImage(x, oPic + "Denoised.png");

        compareImage(origin, x);
    }

    /**
     * Calculate all of the energy
     * @param x  the process image
     * @param y  the observe image
     * @return the global energy
     */
    public static double calculateEnergy(double h, double b, double n, int[][] x, int[][] y) {
        int HEIGHT = y.length;
        int WIDTH = y[0].length;

        int sumh = 0, sumb = 0, sumn = 0, xx, yy;
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                sumh += x[i][j];
                sumn += y[i][j] * x[i][j];
                for (int k = 0; k < 4; ++k) {
                    xx = i + adjacent4[k][0];
                    yy = j + adjacent4[k][1];
                    if(xx >= 0 && xx < HEIGHT && yy >= 0 && yy < WIDTH) {
                        sumb += x[i][j] * x[xx][yy];
                    }
                }
            }
        }
        return h * (double)sumh - b * (double)sumb - n * (double)sumn;
    }

    /**
     * Calculate three lines' energy
     * @param x  the process image
     * @param y  the observe image
     * @param xx the x-coordinate of the processing point
     * @param yy the y-coordinate of the processing point
     * @param flag equals to 1 always
     * @return the partial energy
     */
    public static double calculatePartEnergy(int[][] x, int[][] y, int xx, int yy, int flag, double H, double B, double N) {

        int HEIGHT = x.length;
        int WIDTH = x[0].length;
        double energy = 0;
        energy += H * x[xx][yy];
        if (xx != 0) {
            energy -= B * x[xx - 1][yy] * x[xx][yy];
            if (flag == 1) {
                energy += calculatePartEnergy(x, y, xx - 1, yy, 2, H, B, N);
            }
        }
        if (yy != 0) {
            energy -= B * x[xx][yy - 1] * x[xx][yy];
            if (flag == 1) {
                energy += calculatePartEnergy(x, y, xx, yy - 1, 2, H, B, N);
            }
        }
        if (xx != HEIGHT - 1) {
            energy -= B * x[xx + 1][yy] * x[xx][yy];
            if (flag == 1) {
                energy += calculatePartEnergy(x, y, xx + 1, yy, 2, H, B, N);
            }
        }
        if (yy != WIDTH - 1) {
            energy -= B * x[xx][yy + 1] * x[xx][yy];
            if (flag == 1) {
                energy += calculatePartEnergy(x, y, xx, yy + 1, 2, H, B, N);
            }
        }
        energy -= N * y[xx][yy] * x[xx][yy];
        return energy;
    }
    /**
     * Compare two image matrix and output the propotion of same pixel
     * @param origin original image
     * @param denoised denoised image
     */
    public static void compareImage(int[][] origin, int[][] denoised) {
        double cnt = 0.0;
        int HEIGHT = origin.length;
        int WIDTH = origin[0].length;
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (origin[i][j] == denoised[i][j]) {
                    ++cnt;
                }
            }
        }
        cnt = cnt * 100.0 / (HEIGHT * WIDTH);
        result += cnt;
        System.out.println(cnt);
    }

	public static int[][] readImage(String name) {
        File filePath = new File(name);
        try {
            BufferedImage imgBuffer = ImageIO.read(filePath);
            int height = imgBuffer.getHeight();
            int width = imgBuffer.getWidth();
            int[][] inputImage = new int[width][height];
            int rgb, gray;
            for (int i = 0; i < width; ++i) {
                for (int j = 0; j < height; ++j) {
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

    public static void writeImage(int[][] input_img, String name)  {
        int row = input_img.length;
        int col = input_img[0].length;
        try {
            BufferedImage outFile = new BufferedImage(row, col, BufferedImage.TYPE_INT_RGB);
            int temp;
            for (int i = 0; i < row; ++i) {
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

    private static double result = 0.0;
    private static int[][] adjacent4 = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    private static int[][] adjacent8 = {{-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {1, 0}, {0, -1}, {0, 1}};
}