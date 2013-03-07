/* slice-pattern.js - Draw a colorful pattern on a canvas
 * Copyright (C) 2013  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

function debug (message) {

	$("#debug").append ("<p>" + message + "</p>");
}

function inRange (v, min, max) {

	while (v < min) {
		v += Math.abs (max - min + 1);
	}

	while (v > max) {
		v -= Math.abs (max - min + 1);
	}

	return v;
}

function normalizeAngle (angle) {

	return inRange (angle, 0, 359);
}

function toRadians (degrees) {

	return degrees / 180 * Math.PI;
}

function drawBeachBall (args) {

	var slice;
	var start;
	var end;
	var i;

	if (args.slices % 2 != 0) {
		debug ("Number of slices must be even!");
		return;
	}

	slice = 360 / args.slices;

	start = normalizeAngle (args.offset);
	end = normalizeAngle (start + slice);

	for (i = 0; i < args.slices; i++) {

		var color = args.colors[inRange (i, 0, 1)];

		debug ("start=" + start + ", end=" + end);

		args.canvas.drawArc ({
			strokeStyle: color,
			fillStyle: color,
			closed: true,
			x: args.origin.x,
			y: args.origin.y,
			radius: args.radius,
			start: start,
			end: end
		});

		var x2 = args.origin.x + args.radius * Math.sin(toRadians (start));
		var y2 = args.origin.y - args.radius * Math.cos(toRadians (start));
		var x3 = args.origin.x + args.radius * Math.sin(toRadians (end));
		var y3 = args.origin.y - args.radius * Math.cos(toRadians (end));

		debug ("(" + args.origin.x + "," + args.origin.y + ") (" + x2 + "," + y2 + ") (" + x3 + "," + y3 + ")");

		args.canvas.drawLine ({
			fillStyle: color,
			closed: true,
			x1: args.origin.x,
			y1: args.origin.y,
			x2: x2,
			y2: y2,
			x3: x3,
			y3: y3
		});

		start = normalizeAngle (start + slice);
		end = normalizeAngle (end + slice);
	}
}

function main() {

	var canvas;
	var radius;
	var slices;
	var step;
	var offset;
	var colors;

	slices = 20;
	step = 100;
	offset = 90;

	canvas = $("canvas");

	radius = Math.max (canvas.width (), canvas.height ());
	if (radius % step > 0) {
		radius -= radius % step;
		radius += step;
	}

	while (radius > 0) {

		if ((radius / step) % 2 == 0) {
			colors = ["#003040", "#F22477"];
		}
		else {
			colors = ["#F22477", "#003040"];
		}

		drawBeachBall ({
			canvas: canvas,
			origin: {
				x: canvas.width () / 2,
				y: canvas.height () / 2
			},
			radius: radius,
			slices: slices,
			offset: offset,
			colors: colors
		});

		radius -= step;
	}
}

$(document).ready (main);
